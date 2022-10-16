/**
 *   @file sonar.cpp
 *   @author Huy and Richard
 *   @date 9-June-2022
 *   @brief Source code implemneting a function to setup and use HC-SR04 ultra-sonic sensor.
 * 
 */

#include "sonar.h"

void setupSonar() {
    // Set compare mode to clear on compare match set at bottom
    // Select FAST PWM 
    // Select prescaler of 8
    TCCR4A = 0b10000010;
    TCCR4B = 0b00011010;

    // ICR4 and OCR4A so that PWM 
    // create a 10us pulse every 20ms
    ICR4 = 20000;
    OCR4A = 10;
    DDRH = setBit(DDRH, 0b00001000);

    // enable pin change interrupt for 10 pin
    PCMSK0 = 0b00010000;
    PCICR = 0b00000001;
}