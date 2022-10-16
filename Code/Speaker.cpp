/**
 *   @file Speaker.cpp
 *   @author Huy and Richard
 *   @date 16-May-2022
 *   @brief Source code of the implemention of functions to interface with a speaker.
 */

#include "Speaker.h"

void setupSpeaker(){
    // assign output mode for pin 6 (timer OC4A)
   DDRH = setBit(DDRH, 0b00001000);
   // set timer mode to CTC
   // set clock select to cpu clock with prescalar = 1
   TCCR4B = 0b00001001;
   // initially set compare to off mode
   TCCR4A = 0b00000000;
   // initialize value to compare to 0
   OCR4A = 0;
}

void createTone(int freq){
    if (freq < 123) {
        TCCR4A = 0;
    } else {
        // compute the compare value for given frequency
        uint16_t compare = 8000000/freq - 1;
        OCR4A = compare;
        // set compare mode to toggle
        TCCR4A = setBit(TCCR4A, 0b01000000);
    }
}

void stopTone(){
    TCCR4A = clearBit(TCCR4A, 0b01000000);
}
