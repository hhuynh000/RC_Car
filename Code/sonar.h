/**
 *   @file sonar.h
 *   @author Huy and Richard
 *   @date 9-June-2022
 *   @brief Declaration of function to setup and use HC-SR04 ultra-sonic sensor.
 * 
 */

#ifndef sonar_h
#define sonar_h

#include "utils.h"

/**
 * @brief Setup the HC-SR04 ultra-sonic sensor to get a distance every 20ms.
 * 
 * Setup Timer4A on pin 6 to generate a fast PMW signal with a frequecy of
 * 50Hz and duty cycle of 0.0005%. This is equivalent to a pulse signal lasting
 * 10us every 20ms. This fast PMW signal is used to drive the trigger pin of 
 * HC-SR04 and have it send a new sound wave every 20ms. Assume that it will
 * take faster than 20ms to recieve the signal back. Setup pin change interrupt
 * on pin 10 correspond echo pin on HC-SR04, used to time the pulse signal
 * non-blocking.
 */
void setupSonar();

#endif // sonic_h