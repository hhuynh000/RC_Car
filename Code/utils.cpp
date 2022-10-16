/**
 *   @file utils.cpp
 *   @author Huy and Richard
 *   @date 16-May-2022
 *   @brief Source code for the implemention of utils.h functions
 */

#include "utils.h"

uint16_t setBit(uint16_t input, uint16_t mask) {
    return input | mask;
}

uint16_t clearBit(uint16_t input, uint16_t mask) {
    return input & ~mask;
}

uint16_t setClearBit(uint16_t input, uint16_t maskSet, uint16_t maskClear) {
    uint16_t result = setBit(input, maskSet);
    return clearBit(result, maskClear);
}

uint16_t compBit(uint16_t input, uint16_t mask){
    return input ^ mask;
}

void startFunction (functionPtr function) {
    function();
}