/**
 *   @file tasks.cpp
 *   @author Huy and Richard
 *   @date 16-May-2022
 *   @brief Source code for the implementation of tasks 1-2
 */

#include "tasks.h"

int theme[5] = {293, 329, 261, 130, 196};

void setupTask1() {
    DDRL = setBit(DDRL, 0b000000001); // set first bit of DDRL to put pin 49 in output mode
    PORTL = 0; // initial set LED on pin 49 to be on
}

void task1() {
    // each count is 250 ms
    static int count = 0;
    if (count % 4 == 0) {
        PORTL = setBit(PORTL, 0b00000001);
    } else if (count % 1 == 0) {
        PORTL = clearBit(PORTL, 0b00000001);
    }
    count++;
}

void task2() {
    // each increment is 250ms
    static int sleep = 0;
    static int i = 0;
    if (sleep > 0) {
        sleep--;
        return;
    }
    createTone(theme[i]);
    i++;
    if (i>THEME_NOTES) {
        sleep = 16;
        i = 0;
        stopTone();
    }
}

