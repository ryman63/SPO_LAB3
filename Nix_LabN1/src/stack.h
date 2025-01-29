#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 1024 // Размер стека

// Структура для хранения кадра стека
typedef struct {
    char function_name[32]; // Имя функции
    int return_address;     // Адрес возврата
    int parameters[4];      // Параметры функции (максимум 4)
    int local_variables[8]; // Локальные переменные (максимум 8)
} StackFrame;

// Структура для стека вызовов
typedef struct {
    StackFrame frames[STACK_SIZE]; // Массив кадров стека
    int top;                       // Индекс верхнего кадра
} CallStack;
