/**
 *   @file TCB.h
 *   @author Huy and Richard
 *   @date 21-May-2022
 *   @brief Declaration of task control block and function.
 *   
 */

#ifndef TCB_h
#define TCB_h

#include "utils.h"

#define STATE_RUNNING 0
#define STATE_READY 1
#define STATE_WAITING 2
#define STATE_DEAD 3

/**
 * @brief Task control block struct.
 * 
 * Contain the pointer to associated function, the state of the task, sleep counter,
 * corresponding ID, name given by user and a run count.
*/
typedef struct TCBstruct {
    functionPtr ptr;
    unsigned int state;
    int sleepCount;
    unsigned int ID;
    char* name;
    int runCount;
};

#endif // TCB_h