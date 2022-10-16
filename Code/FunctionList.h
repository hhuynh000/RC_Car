/**
 *   @file FunctionList.h
 *   @author Huy and Richard
 *   @date 21-May-2022
 *   @brief Declaration of a FunctionList and associative functions.
 *   
 *   A FunctionList composes of an array of function pointers and int size.
 *   Include basic functions to setup, add function to the end of the list and
 *   remove a function at a specified index.
 */

#ifndef FunctionList_h
#define FunctionList_h

#include "utils.h"

#define MAX_FUNCTIONS 10

/**
 * @brief Define a FunctionList struct used to manage a list of function pointers.
 * 
 * Contains and array of function pointers with a max size of 10 and 
 * the current size of the list.
 */
typedef struct FunctionList {
    void (*functionList[MAX_FUNCTIONS+1])();
    int size;
};

/**
 * @brief Initilizes a FunctionList.
 * 
 * @param list A pointer to the FunctionList.
 */
void setupFunctionList(FunctionList *list);

/**
 * @brief Add a function pointer to a given FunctionList.
 * 
 * If the FunctionList is full return and does not add the function.
 * 
 * @param function A pointer to a function.
 * @param list A pointer to a FunctionList.
 */
void addFunctionList(void (*function)(), FunctionList *list);

/**
 * @brief Remove a function pointer from a FuntionList given its index.
 * 
 * If the given index
 * @param index An integer indicating an index in the list.
 * @param list A pointer to a FunctionList.
 * @return The 
 */
functionPtr removeFunctionList(int index, FunctionList *list);

/**
 * @brief Get a function pointer from a FuntionList given its index.
 * 
 * @param index An integer indicating an index in the list.
 * @param list A pointer to a FunctionList.
 */
functionPtr getFunctionList(int index, FunctionList *list);

#endif // FunctionList_h