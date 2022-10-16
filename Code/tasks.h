/**
 *   @file tasks.h
 *   @author Huy and Richard
 *   @date 16-May-2022
 *   @brief Declaration of functions to preform tasks 1-2.
 */

#ifndef tasks_h
#define tasks_h

#include "Speaker.h"

#define THEME_NOTES 5

/**
 * @brief Store the notes of the theme.
 */
extern int theme[THEME_NOTES];

/**
 * @brief Setup DDRL register for task 1.
 * 
 * Set DDRL first bit, which configure pin 49 to be an output pin.
 * Initially reset the pin output to be 0.
 */
void setupTask1();

/**
 * @brief Preform task 1: Flash an external LED on pin 49 for 250ms every 1 second.
 * 
 * The LED will be on for 250ms and off for 750ms.
 */
void task1();

/**
 * @brief Play the theme from "Close Encounters of the Third Kind" every 4 second.
 * 
 * Play the entire theme and then sleep for 4 second, then repeat. The speaker is driven
 * by pin 6. Must run setupSpeaker in setup to run task2.
 */ 
void task2();

#endif // tasks_h
