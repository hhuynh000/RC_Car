/**
 *   @file DDS.h
 *   @author Huy and Richard
 *   @date 21-May-2022
 *   @brief Declaration of Data-Drive Scheduler.
 *   
 */
#ifndef DDS_h
#define DDS_h

#include "TCB_List.h"
#include "seg7.h"
#include "Speaker.h"
#include "tasks.h"

/**
 * @brief Setup a synchronized round robin scheduler with task control block .
 */
void setupDDS();

/**
 * @brief Run a synchronized round robin scheduler using task control block .
 */
void run_DDS();

/**
 * @brief DDS scheduler underlying operation to manage tasks
 */
void scheduler_DDS();

/**
 * @brief Sync the DDS scheduler with each unit step being 2ms.
 */
void schedule_sync_DDS();

/**
 * @brief Sleep a task for a specified amount of time for DDS implementation.
 * 
 * @param time Amount of time to sleep in ms. Accurracy of sleep is 2ms.
 */
void sleep_474_DDS(int time);

/**
 * @brief Halt a task complete. Must be called inside a currently running task.
 * 
 * The task is remove from the task TCB array and moved into the graveyard TCB array.
 */
void task_self_quit();

/**
 * @brief Start a given task from the dead state.
 * 
 * The task is remove from the graveyard TCB array and moved into the task TCB array.
 * 
 * @param tcb A pointer to the TCBstruct to start again.
 */
void startTask(TCBstruct *tcb);

/**
 * @brief Preform task 1: Turn on the LED on pin 49.
 * 
 * LED is on for 250ms and off for 750ms. Cycle through every 1 second.
 */
void task1_DDS();

/**
 * @brief Preform task 2: Play the theme every 4 seconds.
 */
void task2_DDS();

/**
 * @brief Run first everytime task4 start. Manage states variables of the task.
 * 
 * Manages global variable bool play and int themeIndex to play the theme using task2_DDS.
 * Also, uses task_computeDigite and task_displayDigits to display frequencies when the theme
 * is playing using global variable number to change the number being displayed.
 * Play is false when task2_DDS is sleeping. When play is false number is set to 4000 which get decrement
 * evey 100ms.
 */
void task4_Start_DDS();

/**
 * @brief Last task to run for task4. 
 * 
 * Increment themeIndex when play is true and decrement number when play is false.
 */
void task4_End_DDS();

/**
 * @brief Preform a subtask of task 5: Blink LED same as task1_DDS.
 */
void task5_1_DDS();

/**
 * @brief Preform a subtask of task 5.
 * 
 * Mange playing the theme twice, then self quit the tesk and start task5_3 and task5_4
 * to display count down from 3000. When revived play one more time then start task5_5
 * to display smile.
 */
void task5_2_DDS();

/**
 * @brief Preform a subtask of task 5.
 * 
 * Compute the digit to display count down from 3000.
 * When the count reaches 0, self quite the task and revive task5_2.
 */
void task5_3_DDS();

/**
 * @brief Preform a subtask of task 5.
 * 
 * Display the  digit counting down from 3000.
 * Self quite after task5_3 stop.
 */
void task5_4_DDS();

/**
 * @brief Preform a subtask of task 5.
 * 
 * Display a smile for 2 second, then self quit the task.
 */
void task5_5_DDS();

/**
 * @brief Compute the digits of a given global variable number.
 * 
 * Store the converted digits in a global digits array. Number is incremented every 100ms.
 * Uses convertDigits() from seg7.h
 */
void task_computeDigits();

/**
 * @brief Display a number using the global digits array.
 * 
 * Uses displayDigit() from seg7.h
 */
void task_DisplayDigits();

/**
 * @brief Display a smile for 2 second then stop.
 * 
 * Uses displaySegment() from seg7.h
 */
void task_smile();

#endif // DDS_h