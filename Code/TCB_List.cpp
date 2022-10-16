/**
 *   @file TCB_List.cpp
 *   @author Huy and Richard
 *   @date 21-May-2022
 *   @brief Source code implementing list of TCB and associated function.
 *   
 */

#include "TCB_List.h"

void setupTCB_List(TCB_List* tcbList) {
    tcbList->size = 0;
    tcbList-> ID_counter = 0;
}

void addTask(functionPtr function, char name[21], TCB_List* tcbList) {
    TCBstruct tcb;
    tcb.name = name;
    tcb.ptr = function;
    tcb.runCount = 0;
    tcb.sleepCount = 0;
    tcb.state = STATE_READY;
    tcb.ID = tcbList->ID_counter;
    tcbList->ID_counter++;
    tcbList->list[tcbList->size] = tcb;
    tcbList->size++;
}

void addTCB(TCBstruct tcb, TCB_List *tcbList) {
    tcbList->list[tcbList->size] = tcb;
    tcbList->size++;
}

TCBstruct removeTCB(TCBstruct tcb, TCB_List* tcbList) {
    int index = 0;
    for (int i = 0; i < tcbList->size; i++) {
        if (tcb.ID == tcbList->list[i].ID) {
            index = i;
        }
    }
    // shift all items to the left to fill in the removed item
    TCBstruct result = tcbList->list[index];
    for (int i = index; i < tcbList->size - 1; i++) {
        tcbList->list[i] = tcbList->list[i+1];
    }
    tcbList->size--;
    return tcb;
}