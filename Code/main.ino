/**
 *   @file main.cpp
 *   @author Huy and Richard
 *   @date 9-June-2022
 *   @brief Main Arduino loop to implement project.
 * 
 */

#include "sonar.h"
#include "Arduino_FreeRTOS.h"
#include "queue.h"
#include <IRremote.h>

#define true 1
#define false 0

volatile unsigned long start, time;
volatile unsigned int running;
QueueHandle_t DistQueue;
unsigned long cm, code;
unsigned int turnLeft, turnRight, control;

/**
 * @brief Pin change interrupt on pin 10 to time pulse input from echo pin.
 * 
 * Uses volatile variable start, time to compute how long the pulse last.
 * The volatile variable running indicate if the a sound wave has been sent out
 * but not recieve back yet.
 */
ISR(PCINT0_vect) {
    if (running) {
        time = micros() - start;
        running = false;
    } else {
        start = micros();
        running = true;
    }
}

#define RECIEVE_PIN1 10
#define MOTION_LED_L_PIN 44
#define MOTION_LED_R_PIN 43
#define MAX_SIZE 10
#define IN1 49
#define IN2 48
#define IN3 47
#define IN4 46
#define RECV_PIN 7

// IR remote
IRrecv irrecv(RECV_PIN);
decode_results results;

/**
 * @brief Get and convert the pulse time from HC-SR04 to cm (if avaliable), then send data to queue.
 * 
 * Convert the time to cm using microsecondsToCentimeters. Create a FreeRTOS queue, DistQueue 
 * to store 10 distance data
 */
void getDistance(void *pvParameters);

/**
 * @brief Dectect an IR signal and convert the appropiate button press to turn left or right.
 * 
 * The play backward button is turn right and play foward button is turn left.
 * Take new input every second. In addition, blink the appropiate right or left LEDS
 * for 1 second.
 */
void getRemote(void *pvParameters);

/**
 * @brief Based on data from sensor determine the robot next movement.
 * 
 * If turn left or right if triggered by IR remote (about 90 degree turn).
 * Otherwise check if there is an object in range (20cm - 120cm) go straight.
 * Stop if there is no object in range. The distance is computed by taking the average
 * of 10 data points from the DistQueue. Update every 100ms.
 */
void computeMotion(void *pvParameters);

/**
 * @brief Convert a time in ms to distance in cm based on speed of sound.
 * 
 */
long microsecondsToCentimeters(long microseconds);

/**
 * @brief Configure control pins for motors to turn right by 90 degree.
 */
void steerRight();

/**
 * @brief Configure control pins for motors to turn left by 90 degree.
 */
void steerLeft();

/**
 * @brief Configure control pins for motors to turn right by 90 degree.
 */
void straight();

/**
 * @brief Configure control pins for motors to not move.
 */
void idle();

void setup() {
  // initialize remote IR

  irrecv.enableIRIn();
  irrecv.blink13(true);

  // initialize ultra sound
  setupSonar();
  pinMode(RECIEVE_PIN1, INPUT);
  running = false;
  Serial.begin(9600);

  // for LED with PIR sensor
  pinMode(MOTION_LED_R_PIN, OUTPUT);
  pinMode(MOTION_LED_L_PIN, OUTPUT);

  // motor pins control direction of wheels
  // IN1-2 control right wheels
  // IN3-4 control left wheel
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // All tasks are same priority, want to be done sequentially
  xTaskCreate(
    getDistance
    ,  "Distance" 
    ,  256  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
  );

  xTaskCreate(
    getRemote
    ,  "PIR" 
    ,  256  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
  );

  xTaskCreate(
    computeMotion
    ,  "Motion" 
    ,  512  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
  );

  vTaskStartScheduler();
}

void loop() {
  // freeRTOS scheduler run tasks
}

void getDistance(void *pvParameters) {
  DistQueue = xQueueCreate(MAX_SIZE, sizeof(long));
  for (;;) {
    if (!running) {
      cm = microsecondsToCentimeters(time);
      xQueueSendToBack(DistQueue, &cm, (TickType_t) 0);
    }
  }
}

void getRemote(void *pvParameters) {

  for( ;;) {
    code = 0;
    // If there is an IR signal, decode the signal
    if (irrecv.decode(&results)){
          code = results.value;
          Serial.println(code);
          irrecv.resume();
    }

    // the recieved code is play backward button
    if (code == 0xff22dd) {
      turnLeft = true;
      digitalWrite(MOTION_LED_L_PIN, HIGH);
      vTaskDelay(1000 / portTICK_PERIOD_MS );
    } else if (code == 0xffc23d) {    // the recieved code is play foward button
      turnRight = true;
      digitalWrite(MOTION_LED_R_PIN, HIGH);
      vTaskDelay(1000 / portTICK_PERIOD_MS );
    } else {
      turnLeft = false;
      turnRight = false;
      digitalWrite(MOTION_LED_R_PIN, LOW);
      digitalWrite(MOTION_LED_L_PIN, LOW);
    }
  }
}

void computeMotion(void *pvParameters) {
  unsigned long avgDist, buff;
  unsigned int inRange;
  unsigned int counter = 0;
  inRange = 0;
  avgDist = 0;
  buff = 0;
  for ( ;; ) {
    avgDist = 0;
    counter = MAX_SIZE;
    // does not allow interrupt to occur
    noInterrupts();
    // Compute the average of 10 distance data point from queue
    while (counter > 0) {
      xQueueReceive(DistQueue, &buff, (TickType_t) 0);
      avgDist += buff;
      counter--;
    }
    avgDist = avgDist/MAX_SIZE;
    // Check if there is an object in range
    if (avgDist < 120 && avgDist > 20) {
      inRange = true;
    } else {
      inRange = false;
    }

    // Based on sensor data determine the next movement
    if (turnRight) {
      steerRight();
      vTaskDelay(500 / portTICK_PERIOD_MS );

    } else if (turnLeft) {
      steerLeft();
      vTaskDelay(500 / portTICK_PERIOD_MS );

    } else if (inRange) {
      straight();

    } else {
      idle();
    }

    interrupts();
    vTaskDelay(100 / portTICK_PERIOD_MS );
  }
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

void steerRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  turnRight = false;
}

void steerLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  turnLeft = false;
}

void straight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void idle() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}