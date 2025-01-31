#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Variable.h"

#define STACK_SIZE 1024 // ������ �����
#define MAX_LOCAL_VARIABLE 64 // ������������ ���-�� ��������� ����������

// ��������� ��� �������� ����� �����
typedef struct {
    int return_address;     // ����� ��������
    Variable localVars[MAX_LOCAL_VARIABLE];
    ProgramUnit* programUnit;
} StackFrame;

// ��������� ��� ����� �������
typedef struct {
    StackFrame frames[STACK_SIZE]; // ������ ������ �����
    int top;                       // ������ �������� �����
} CallStack;

void initCallStack(CallStack* stack);
int isEmpty(CallStack* stack);
int isFull(CallStack* stack);
void pushFrame(CallStack* stack, int return_address, ProgramUnit* programUnit);
StackFrame popFrame(CallStack* stack);
StackFrame peekFrame(CallStack* stack);
