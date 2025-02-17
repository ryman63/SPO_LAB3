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
void pushFrame(CallStack* stack, ProgramUnit* programUnit) {
    if (isFull(stack)) {
        printf("Ошибка: стек вызовов переполнен!\n");
        return;
    }

    // Создаём новый кадр
    StackFrame frame;

    //frame.return_address = return_address;
    frame.programUnit = programUnit;
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
