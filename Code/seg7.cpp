/**
 *   @file seg7.cpp
 *   @author Huy and Richard
 *   @date 16-May-2022
 *   @brief Source code for the implemention of 7 segment display functions.
 */

#include "seg7.h"

// Pins:
//      30 - DP
//      31 - G
//      32 - F
//      33 - E
//      34 - D
//      35 - C
//      36 - B  
//      37 - A

// Pins:
//      42 - DIG1
//      43 - DIG2
//      44 - DIG3
//      45 - DIG4

uint8_t seven_seg_digits[10] = { 0b00111111,  // = 0
                                 0b00000110,  // = 1
                                 0b01011011,  // = 2
                                 0b01001111,  // = 3
                                 0b01100110,  // = 4
                                 0b01101101,  // = 5
                                 0b01111101,  // = 6
                                 0b00000111,  // = 7
                                 0b01111111,  // = 8
                                 0b01100111   // = 9
                                            };


void setupSeg7() {
    DDRL = setBit(DDRL, 0b11110000);
    DDRC = setBit(DDRC, 0b11111111);
}

void displayDigit(int digit) {
    PORTC = seven_seg_digits[digit];
}

void displaySegment(uint8_t segments) {
    PORTC = segments;
}

void convertDigit(int number, int* digits) {
    // allocate memory for array size of 4 to hold 4 digits
    // if the given number > 9999 overflow back to 0
    if (number > 9999) {
        number = number % 9999;
    }
    // use modulo and division to get each digit place 
    for (int i = 0; i < 4; i++) {
        int digit = number % 10;
        if (number == 0 && digit == 0) {

            digits[i] = -1;
        } else {
            digits[i] = digit;
        }
        number = number / 10;
    }
}
