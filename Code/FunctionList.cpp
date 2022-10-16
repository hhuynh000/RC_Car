/**
 *   @file FunctionList.cpp
 *   @author Huy and Richard
 *   @date 21-May-2022
 *   @brief Source code implementing FunctionList and associative functions.
 *   
 *   A FunctionList composes of an array of function pointers and int size.
 *   Include basic functions to setup, add function to the end of the list and
 *   remove a function at a specified index.
 */

#include "FunctionList.h"

void setupFunctionList(FunctionList *list) {
    list->size = 0;
    list->functionList[0] = NULL;
}

void addFunctionList(void (*function)(), FunctionList *list) {
    if (list->size >= MAX_FUNCTIONS) {
        return;
    }
    list->functionList[list->size] = function;
    list->functionList[list->size+1] = NULL;
    list->size++;
}

functionPtr removeFunctionList(int index, FunctionList *list) {
    if (index >= list->size) {
        return NULL;
    }
    functionPtr result = list->functionList[index];
    while (list->functionList[index] != NULL) {
        list->functionList[index] = list->functionList[index+1];
        index++;
    }
    list->functionList[index-1] = NULL; 
    list->size--;
    return result;
}

functionPtr getFunctionList(int index, FunctionList *list) {
    if (index >= list->size) {
        return NULL;
    }
    return list->functionList[index];
}