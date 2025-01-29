#include "stack.h"

// Инициализация стека вызовов
void initCallStack(CallStack* stack) {
    stack->top = -1; // Указывает, что стек пуст
}

// Проверка, пуст ли стек вызовов
int isEmpty(CallStack* stack) {
    return stack->top == -1;
}

// Проверка, полон ли стек вызовов
int isFull(CallStack* stack) {
    return stack->top == STACK_SIZE - 1;
}

// Добавление нового кадра в стек вызовов (вызов функции)
void pushFrame(CallStack* stack, const char* function_name, int return_address, int* params, int param_count) {
    if (isFull(stack)) {
        printf("Ошибка: стек вызовов переполнен!\n");
        return;
    }

    // Создаём новый кадр
    StackFrame frame;
    //strncpy(frame.function_name, function_name, 32); // Имя функции
    frame.return_address = return_address;           // Адрес возврата

    // Инициализация параметров
    for (int i = 0; i < param_count && i < 4; i++) {
        frame.parameters[i] = params[i];
    }

    // Инициализация локальных переменных (обнуляем)
    for (int i = 0; i < 8; i++) {
        frame.local_variables[i] = 0;
    }

    // Добавляем кадр в стек
    stack->frames[++stack->top] = frame;
}

// Удаление кадра из стека вызовов (завершение функции)
StackFrame popFrame(CallStack* stack) {
    if (isEmpty(stack)) {
        printf("Ошибка: стек вызовов пуст!\n");
        StackFrame error_frame = { "<empty>", -1, {0}, {0} };
        return error_frame;
    }
    return stack->frames[stack->top--];
}

// Просмотр верхнего кадра стека вызовов
StackFrame peekFrame(CallStack* stack) {
    if (isEmpty(stack)) {
        printf("Ошибка: стек вызовов пуст!\n");
        StackFrame error_frame = { "<empty>", -1, {0}, {0} };
        return error_frame;
    }
    return stack->frames[stack->top];
}
