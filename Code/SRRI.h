/**
 *   @file SRRI.h
 *   @author Huy and Richard
 *   @date 21-May-2022
 *   @brief Declaration of synchronized round robin sceduler with ISR.
 */

#include "tasks.h"
#include "FunctionList.h"
#include "seg7.h"

/**
 * @brief Setup a synchronized round robin scheduler with ISR .
 */
void setupSRRI();

/**
 * @brief Run a synchronized round robin scheduler with ISR .
 */
void run_SRRI();

/**
 * @brief Scheduler underlying operation to manage tasks
 */
void scheduler_SRRI();

/**
 * @brief Sync scheduler with each unit step being 2ms.
 */
void schedule_sync();

/**
 * @brief Sleep a task for a specified amount of time.
 * 
 * @param time Amount of time to sleep in ms. Accurracy of sleep is 2ms unit.
 */
void sleep_474(int time);

/**
 * @brief Shift the sleep count to align with sleepTask array after a remove.
 * 
 * @param index Index value of the task removed from sleepTask array.
 */
void shiftSleepCount(int index);

/**
 * @brief Preform task 1: Turn on the LED on pin 49.
 * 
 * LED is on for 250ms and off for 750ms. Cycle through every 1 second.
 */
void task1_SRRI();

/**
 * @brief Preform task 2: Play the theme every 4 seconds.
 */
void task2_SRRI();

/**
 * @brief Preform a subtask of task 3: Compute digits from a given global variable number.
 * 
 * Store the converted digits in a global digits array. Number is incremented every 100ms.
 * Uses convertDigits() from seg7.h
 */
void task3_1_SRRI();

/**
 * @brief Preform a subtask of task 3: Display the number using the global digits array
 * 
 * Uses displayDigit() from seg7.h
 */
void task3_2_SRRI();
