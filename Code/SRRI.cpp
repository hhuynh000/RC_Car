/**
 *   @file SRRI.cpp
 *   @author Huy and Richard
 *   @date 21-May-2022
 *   @brief Source code implementing the synchronized round robin sceduler with ISR.
 */

#include "SRRI.h"

// select Demo
//#define DEMO 1
#define DEMO_5 1

#define DONE 1
#define PENDING 0

volatile unsigned int sFlag_SRRI;

FunctionList readyTasks;
FunctionList runningTasks;
FunctionList sleepingTasks;

int sleepCount[10] = {0};
int digits_SRRI[4] = {-1};
int number_SRRI;

#ifdef DEMO
ISR(TIMER0_COMPA_vect) {
    sFlag_SRRI = DONE;
}
#endif
void setupSRRI() {

    // Setup Timer/Counter0A for interrupt
    DDRB = 0b10000000;
    TCCR0A = 0b01000010; // set to CTC mode and enable toggle mode on compare
    TCCR0B = 0b00000100; // set prescaler of 256
    OCR0A = 124;         // set compare value to get a 250 Hz signal
    TIMSK0 = 0b00000010; // enable interrupt for timer/counter0

    // initilize the lists for ready, running and sleeping states
    setupFunctionList(&readyTasks);
    setupFunctionList(&runningTasks);
    setupFunctionList(&sleepingTasks);

    // initialize interupt flag
    sFlag_SRRI = PENDING;

    // initilize number to display on the 7 segments display to 0
    number_SRRI = 0;

    // set up PORT for 7 segments display
    PORTL = setBit(PORTL, 0b11110000);

    // Add in desired function(s) to run
    #ifdef DEMO_5
    addFunctionList(task3_1_SRRI, &readyTasks);
    addFunctionList(task3_2_SRRI, &readyTasks);
    #endif
    addFunctionList(task1_SRRI, &readyTasks);
    addFunctionList(task2_SRRI, &readyTasks);
}

void run_SRRI() {
    while (1) {
        scheduler_SRRI();
        schedule_sync();
    }
}

//              Scheduler Functions             //
void schedule_sync() {
    if (!sFlag_SRRI) {
        return;
    }

    for (int i = 0; i < sleepingTasks.size; i++) {
        if (sleepCount[i] > 0) {
            sleepCount[i] = sleepCount[i] - 2;
        }
        // remaning time of 1 ms will be rounded down
        // and the task will go off sleep
        if (sleepCount[i] < 2) {
            functionPtr task = removeFunctionList(i, &sleepingTasks);
            shiftSleepCount(i);
            addFunctionList(task, &readyTasks);
        } 
    }

    for (int i = 0; i < runningTasks.size; i++) {
        functionPtr task = removeFunctionList(0, &runningTasks);
        addFunctionList(task, &readyTasks);
    }

    sFlag_SRRI = PENDING;
    return;
}

void scheduler_SRRI() {
    if (readyTasks.functionList[0] == NULL && readyTasks.size == 0) {
        return;
    }
    functionPtr task = removeFunctionList(0, &readyTasks);
    if (task != NULL) {
        addFunctionList(task, &runningTasks);
        startFunction(task);
    }
}

void sleep_474(int time) {
    int sleepIndex = sleepingTasks.size;
    functionPtr task = removeFunctionList(runningTasks.size-1, &runningTasks);
    if (task != NULL) {
        addFunctionList(task, &sleepingTasks);
        sleepCount[sleepIndex] = time;
    }
}

void shiftSleepCount(int index) {
    int i = index;
    while (i < sleepingTasks.size) {
        sleepCount[i] = sleepCount[i+1];
        i++;
    }
    sleepCount[i] = 0;
}


//              Tasks Functions                //
void task1_SRRI() {
    int mask = 0b00000001;
    int compare = mask & PORTL;
    if (compare == 0) {
        PORTL = setBit(PORTL, 0b00000001);
        sleep_474(250);
    } else {
        PORTL = clearBit(PORTL, 0b00000001);
        sleep_474(750);
    }
}

void task2_SRRI() {
    static int index = 0;
    if (index == 5) {
        index = 0;
        sleep_474(4000);
        stopTone();
        return;
    }
    createTone(theme[index]);
    index++;
    sleep_474(250);
}


void task3_1_SRRI() {
    convertDigit(number_SRRI, digits_SRRI);
    // count up every 100ms
    number_SRRI++;
    sleep_474(100);
}

void task3_2_SRRI() {
    static bool wait = true;
    static int displayIndex = 0;
    static uint8_t mask = 0b00010000;

    if (displayIndex== 4) {
        displayIndex = 0;
        PORTL = setBit(PORTL, 0b11110000);
        mask = 0b00010000;
    }

    if (wait) {
        if (digits_SRRI[displayIndex] >= 0) {
            PORTL = clearBit(PORTL, mask);
            displayDigit(digits_SRRI[displayIndex]);
        }
        wait = false;
        return;
    }

    PORTL = setBit(PORTL, mask);
    mask = mask << 1;
    displayIndex++;
    wait = true;
}