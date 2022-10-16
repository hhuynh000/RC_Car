/**
 *   @file RR.cpp
 *   @author Huy and Richard
 *   @date 21-May-2022
 *   @brief Source code implemention of Round Robin Scheduler.
 */

#include "RR.h"

void scheduler_RR() {
    while(1) {
        task1();
        task2();
        delay(250); // time unit is set to 250ms
    }
}