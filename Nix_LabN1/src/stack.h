#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 1024 // ������ �����

// ��������� ��� �������� ����� �����
typedef struct {
    char function_name[32]; // ��� �������
    int return_address;     // ����� ��������
    int parameters[4];      // ��������� ������� (�������� 4)
    int local_variables[8]; // ��������� ���������� (�������� 8)
} StackFrame;

// ��������� ��� ����� �������
typedef struct {
    StackFrame frames[STACK_SIZE]; // ������ ������ �����
    int top;                       // ������ �������� �����
} CallStack;
