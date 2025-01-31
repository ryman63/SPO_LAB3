#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Variable.h"

#define STACK_SIZE 1024 // Размер стека
#define MAX_LOCAL_VARIABLE 64 // Максимальное кол-во локальных переменных

// Структура для хранения кадра стека
typedef struct {
    int return_address;     // Адрес возврата
    Variable localVars[MAX_LOCAL_VARIABLE];
    ProgramUnit* programUnit;
} StackFrame;

// Структура для стека вызовов
typedef struct {
    StackFrame frames[STACK_SIZE]; // Массив кадров стека
    int top;                       // Индекс верхнего кадра
} CallStack;

void initCallStack(CallStack* stack);
int isEmpty(CallStack* stack);
int isFull(CallStack* stack);
void pushFrame(CallStack* stack, int return_address, ProgramUnit* programUnit);
StackFrame popFrame(CallStack* stack);
StackFrame peekFrame(CallStack* stack);
