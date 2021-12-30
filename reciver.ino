/* Receiver code for the Arduino Radio control 
    
    GND    ->   GND
    Vcc    ->   3.3V
    CE     ->   D9
    CSN    ->   D10
    CLK    ->   D13
    MOSI   ->   D11
    MISO   ->   D12

Motor 1

    PMW    ->   D6
    Direction   ->   D5

Motor 2

    PMW    ->   D3
    Direction   ->   D2
*/


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define rightMotor 6
#define leftMotor 3
const uint64_t pipeIn = 0xE8E8F0F0E1LL;     
RF24 radio(9, 10); 
 

// The sizeof this struct should not exceed 32 bytes
struct Received_data {
  byte ch1;
  byte ch2;
};

Received_data received_data;


void reset_the_Data() 
{
  received_data.ch2 = 129;      
  received_data.ch1 = 126;
}

bool flag = LOW;
void setup()
{
  Serial.begin(9600);
  reset_the_Data();
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);  
  radio.openReadingPipe(1,pipeIn);
  radio.startListening();
  
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT); 
  pinMode(6, OUTPUT); 
  digitalWrite(5, LOW);
  digitalWrite(2, HIGH);
}

////////////////////////////////////////////////////////////////

unsigned long lastRecvTime = 0;


void receive_the_data() {
  while (radio.available()) {
    radio.read(&received_data, sizeof(Received_data));
    lastRecvTime = millis(); 
  }
}


void loop() {
  
  receive_the_data();
  unsigned long now = millis();
  
  // check for receiving signals
  if (now - lastRecvTime > 1000) {
    reset_the_Data();
  } 
  

  int turnData = received_data.ch2 - 129;
  int driveData = received_data.ch1 - 126;
  

  // Testing read values
  if (Serial.available() > 0) {
    Serial.print("T:");
    Serial.println(turnData);
    Serial.print("D:");
    Serial.println(driveData);
  }

  if (turnData < 0 && driveData == 0) { //rotate left
      digitalWrite(5, HIGH);
      digitalWrite(2, HIGH);
      analogWrite(leftMotor, 50);
      analogWrite(rightMotor, 0);
      
  } else if (turnData > 0 && driveData == 0) { //rotate right
      digitalWrite(5, LOW);
      digitalWrite(2, LOW);
      analogWrite(leftMotor, 0);
      analogWrite(rightMotor, 150);
      
  } else if (turnData > 0) { // turn left
    if (driveData > 0) {
      digitalWrite(5, LOW);
      digitalWrite(2, HIGH);
    } else {
      digitalWrite(5, HIGH);
      digitalWrite(2, LOW);
    }
    driveData = abs(driveData);
    int driveSpeed = map(driveData, 0, 126, 255, 200);
    
    analogWrite(leftMotor, driveSpeed);
    analogWrite(rightMotor, 0);
    
   } else if (turnData < 0) { // turn right
     if (driveData > 0) {
      digitalWrite(5, LOW);
      digitalWrite(2, HIGH);
    } else {
      digitalWrite(5, HIGH);
      digitalWrite(2, LOW);
    }
    driveData = abs(driveData);
    int driveSpeed = map(driveData, 0, 126, 255, 200);

    analogWrite(leftMotor, 0);
    analogWrite(rightMotor, driveSpeed);
    
  } else if (driveData > 0) { // backward
      digitalWrite(5, LOW);
      digitalWrite(2, HIGH);
      int driveSpeed = map(driveData, 0, 129, 255, 0);
      analogWrite(rightMotor, driveSpeed);
      analogWrite(leftMotor, driveSpeed);
     
  } else if (driveData < 0) { // foward
  
    digitalWrite(5, HIGH);
    digitalWrite(2, LOW);
    driveData = abs(driveData);
   
    int driveSpeed = map(driveData, 0, 129, 255, 0);
    analogWrite(rightMotor, driveSpeed);
    analogWrite(leftMotor, driveSpeed);
    
  } else if (driveData == 0 && turnData == 0){ // stop/ idle
    analogWrite(rightMotor, 255);
    analogWrite(leftMotor, 255);
  }   
}
