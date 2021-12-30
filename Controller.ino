
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


const uint64_t my_radio_pipe = 0xE8E8F0F0E1LL;

const int SW_pin = 2; // digital pin connected to SW
const int X_pin1 = 0; // analog pin connected to VRx
const int Y_pin1 = 1; // analog pin connected to VRy


RF24 radio(9, 10);

struct Data_to_be_sent {
  byte ch1;
  byte ch2;

};

Data_to_be_sent sent_data;

void setup() {
   radio.begin();
   radio.setAutoAck(false);
   radio.setDataRate(RF24_250KBPS);
   radio.openWritingPipe(my_radio_pipe);

   pinMode(SW_pin, INPUT);
   digitalWrite(SW_pin, HIGH);
   Serial.begin(9600);

   sent_data.ch1 = 127;
   sent_data.ch2 = 127;

   
  
}



void loop() {
  Serial.print("X1: ");
  Serial.print(analogRead(X_pin1));
  Serial.print(" Y1: ");
  Serial.println(analogRead(Y_pin1));


  sent_data.ch1 = map( analogRead(Y_pin1), 0, 1023, 0, 255);
  sent_data.ch2 = map( analogRead(X_pin1), 0, 1023, 0, 255);
  radio.write(&sent_data, sizeof(Data_to_be_sent));

  delay(250);
}
