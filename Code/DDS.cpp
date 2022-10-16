/**
 *   @file DDS.cpp
 *   @author Huy and Richard
 *   @date 21-May-2022
 *   @brief Source code implementation of a Data-Drive Scheduler.
 *   
 */

// run DDS Demo
#define DEMO 1

// select DEMO to run
//#define DEMO_3 1
//#define DEMO_5 1
#define DEMO_6 1

/////////////////////////////////////////////
#define DONE 1
#define PENDING 0

#include "DDS.h"

// interrupt flag
volatile unsigned int sFlag_DDS;

// Task control block lists
TCB_List taskList;
TCB_List graveyard;

// overall index indicating the current running task
int taskIndex;

// variables for 7 segments display
int digits[4] = {-1};
int number;

// variables for task 4
int themeIndex;
bool play;

// variable for task 5
bool revive;

// bit values for displaying smile of 7 segments
uint8_t smileArray[4] = {        0b00001100,  // = _|
                                 0b10101011,  // = _^
                                 0b10101011,  // = ^-
                                 0b10011000,  // = |_
                                            };

// interupt for synchronization with unit time of 2ms
#ifdef DEMO
ISR(TIMER0_COMPA_vect) {
    sFlag_DDS = DONE;
}
#endif

void setupDDS() {
    // Setup Timer/Counter0A for interrupt
    DDRB = 0b10000000;
    TCCR0A = 0b01000010; // set to CTC mode and enable toggle mode on compare
    TCCR0B = 0b00000100; // set prescaler of 256
    OCR0A = 124;         // set compare value to get a 250 Hz signal
    TIMSK0 = 0b00000010; // enable interrupt for timer/counter0

    sFlag_DDS = PENDING;
    taskIndex = 0;

    // initilialize task 4 variable
    themeIndex = 0;
    play = true;

    // initilialize task 5 variable
    revive = false;

    // set up PORT
    PORTL = setBit(PORTL, 0b11110000);

    // setup tasks for scheduler 
    setupTCB_List(&taskList);
    setupTCB_List(&graveyard);

    // Add in desired function(s) to run

    // Demo #3
    #ifdef DEMO_3
    addTask(task1_DDS, "Blink LED", &taskList);
    addTask(task2_DDS, "Play Tone", &taskList);
    #endif

    // Demo #5
    #ifdef  DEMO_5
    addTask(task4_Start_DDS, "Start Cycle", &taskList);
    addTask(task_computeDigits, "Compute Digits", &taskList);
    addTask(task_DisplayDigits, "Display Digits", &taskList);
    addTask(task2_DDS, "Play Theme", &taskList);
    addTask(task4_End_DDS, "End Cycle", &taskList);
    #endif

    // Demo #6
    #ifdef DEMO_6
    addTask(task5_1_DDS, "Flash LED", &taskList);
    addTask(task5_2_DDS, "Play Tone", &taskList);
    #endif
}

void run_DDS() {
    while (1) {
        scheduler_DDS();
        schedule_sync_DDS();
    }
}

//              Scheduler Functions               //
void scheduler_DDS() {
    if (taskIndex >= taskList.size) {
        taskIndex = 0;
    }
    if (taskList.list[taskIndex].state == STATE_READY) {
        taskList.list[taskIndex].state = STATE_RUNNING;
        taskList.list[taskIndex].runCount++;
        startFunction(taskList.list[taskIndex].ptr);
    }
    taskIndex++;
}

void schedule_sync_DDS() {
    if (!sFlag_DDS) {
        return;
    }

    for (int i = 0; i < taskList.size; i++) {
        // Decrement sleep count for current sleeping tasks
        if (taskList.list[i].state == STATE_WAITING) {
            taskList.list[i].sleepCount = taskList.list[i].sleepCount - 2;
        }
        // If task is done sleeping move back to ready
        if (taskList.list[i].sleepCount < 2) {
            taskList.list[i].state = STATE_READY;
        }
        // Move tasks currently running back to ready
        if (taskList.list[i].state == STATE_RUNNING) {
            taskList.list[i].state = STATE_READY;
        }
    }
    sFlag_DDS = PENDING;
}

void sleep_474_DDS(int time) {
    taskList.list[taskIndex].state = STATE_WAITING;
    taskList.list[taskIndex].sleepCount = time;
}

void task_self_quit() {
    taskList.list[taskIndex].state = STATE_DEAD;
    TCBstruct task = removeTCB(taskList.list[taskIndex], &taskList);
    addTCB(task, &graveyard);
    taskIndex--;
}

void startTask(TCBstruct *tcb) {
    TCBstruct task = removeTCB(*tcb, &graveyard);
    task.state = STATE_READY;
    addTCB(task, &taskList);
}

//              Tasks               // 
void task1_DDS() {
    // get first bit
    int compare = 0b00000001 & PORTL;
    if (compare == 0) {
        PORTL = setBit(PORTL, 0b00000001);
        sleep_474_DDS(250);
    } else {
        PORTL = clearBit(PORTL, 0b00000001);
        sleep_474_DDS(750);
    }
}

void task2_DDS() {
    static int toneIndex = 0;
    if (toneIndex == 5) {
        toneIndex = 0;
        stopTone();
        sleep_474_DDS(4000);
        return;
    }
    createTone(theme[toneIndex]);
    toneIndex++;
    sleep_474_DDS(250);
}

void task4_Start_DDS() {
    // when the theme is done playing
    if (themeIndex == 5) {
        themeIndex = 0;
        number = 4000;
        play = false;
    }

    // count down complete indicating to play theme again
    if (number == 0) {
        play = true;
    }

    // if the theme is playing display frequencies
    if (play) {
        number = theme[themeIndex];
        sleep_474_DDS(250); // same timing as the song notes
        return;
    } 
    sleep_474_DDS(100);
}

void task4_End_DDS() {
    // when the theme is playing to on to the next note
    if (play) {
        themeIndex++;
        sleep_474_DDS(250);
    } else {
        // decrement counter when theme is not playing
        number = number - 100;
        sleep_474_DDS(100);
    }
}

void task5_1_DDS() {
    // blink LED
    task1_DDS();
}

void task5_2_DDS() {

    static int themeIndex = 0;
    // played all the 5 notes
    if (themeIndex == 5) {
        stopTone();
        themeIndex = 0;
        // Task two has play the theme 2 times
        if (taskList.list[taskIndex].runCount == 12) {
            number = 3000;
            task_self_quit();
            // start counting down for 3 second
            addTask(task5_3_DDS, "Compute Digits", &taskList);
            addTask(task5_4_DDS, "Display Digitis", &taskList);
            return;
        }
        // End after playing one final time
        if (revive) {
            // add display smile task
            task_self_quit();
            addTask(task5_5_DDS, "Display Smile", &taskList);
            return;
        }
        sleep_474_DDS(4000);
        return;
    }
    // contiue to play each note
    createTone(theme[themeIndex]);
    themeIndex++;
    sleep_474_DDS(250);
}

void task5_3_DDS() {
    // end after done counting down
    if (number == 0) {
        revive = true;
        // play the theme one last time
        startTask(&graveyard.list[0]);
        task_self_quit();
        return;
    }
    convertDigit(number, digits);
    number = number - 100;
    sleep_474_DDS(100);
    return;
}

void task5_4_DDS() {
    // end after done counting down
    if (revive) {
        PORTL = setBit(PORTL, 0b11110000);
        task_self_quit();
        return;
    }
    task_DisplayDigits();
}

void task5_5_DDS() {
    // Every cycle is about 2ms to 2 second is 1000 cylces
    // stop display smile after 2 second
    if (taskList.list[taskIndex].runCount == 1000) {
        PORTL = setBit(PORTL, 0b11110000);
        task_self_quit();
        return;
    }
    task_smile();

}

void task_computeDigits() {
    convertDigit(number, digits);
    sleep_474_DDS(250);
}

void task_DisplayDigits() {
    static int displayIndex = 0;
    static bool wait = true;
    static uint8_t mask = 0b00010000;

    if (displayIndex == 4) {
        displayIndex = 0;
        PORTL = setBit(PORTL, 0b11110000);
        mask = 0b00010000;
    }

    if (wait) {
        if (digits[displayIndex] >= 0) {
            PORTL = clearBit(PORTL, mask);
            displayDigit(digits[displayIndex]);
        }
        wait = false;
        return;
    }

    PORTL = setBit(PORTL, mask);
    mask = mask << 1;
    displayIndex++;
    wait = true;
}

void task_smile() {
    static int displayIndex = 0;
    static bool wait = true;
    static uint8_t mask = 0b00010000;
    if (displayIndex== 4) {
        displayIndex = 0;
        PORTL = setBit(PORTL, 0b11110000);
        mask = 0b00010000;
    }
    if (wait) {
        PORTL = clearBit(PORTL, mask);
        displaySegment(smileArray[displayIndex]);
        wait = false;
        return;
    }
    PORTL = setBit(PORTL, mask);
    mask = mask << 1;
    displayIndex++;
    wait = true;
}
