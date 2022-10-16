/**
 *   @file TCB_List.h
 *   @author Huy and Richard
 *   @date 21-May-2022
 *   @brief Declaration of list of TCB and associated function.
 *   
 */

#ifndef TCB_List_h
#define TCB_List_h

#include "TCB.h"

#define MAX_NUM_TASKS 10

/**
 * @brief A struct to manage a list of task control block.
 * 
 * Contain an array of TCBstruct, the size of the list and an
 * ID counter used to generate unqiue ID for each task added.
 */
typedef struct TCB_List {
    TCBstruct list[MAX_NUM_TASKS];
    int size;
    int ID_counter;
};

/**
 * @brief Inititialize a TCB_List.
 * 
 * @param list A pointer to the TCB_List to be initializes
 */
void setupTCB_List(TCB_List *list);

/**
 * @brief Add a new task to the TCB_List.
 * 
 * @param function A pointer of the function to be added.
 * @param name The name of the function being added (given by the user). Restricted to 20 characters.
 * @param tcbList A pointer to a TCB_List that you want to add the task to.
*/
void addTask(functionPtr function, char name[21], TCB_List *tcbList);

/**
 * @brief Add an already created TCBstruct to a TCB_List.
 * 
 * @param tcb The TCBstruct you want to add.
 * @param tcbList A pointer to a TCB_List that you want to add the TCBstruct to.
*/
void addTCB(TCBstruct tcb, TCB_List *tcbList);

/**
 * @brief Remove and TCBstruct from the TCB_List.
 * 
 * @param tcb The TCBstruct you want to remove.
 * @param tcbList A pointer to a TCB_List that you want to remove from.
*/
TCBstruct removeTCB(TCBstruct tcb, TCB_List *tcbList);

#endif // TCB_List_h