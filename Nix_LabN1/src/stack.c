#include "stack.h"

// ������������� ����� �������
void initCallStack(CallStack* stack) {
    stack->top = -1; // ���������, ��� ���� ����
}

// ��������, ���� �� ���� �������
int isEmpty(CallStack* stack) {
    return stack->top == -1;
}

// ��������, ����� �� ���� �������
int isFull(CallStack* stack) {
    return stack->top == STACK_SIZE - 1;
}

// ���������� ������ ����� � ���� ������� (����� �������)
void pushFrame(CallStack* stack, const char* function_name, int return_address, int* params, int param_count) {
    if (isFull(stack)) {
        printf("������: ���� ������� ����������!\n");
        return;
    }

    // ������ ����� ����
    StackFrame frame;
    //strncpy(frame.function_name, function_name, 32); // ��� �������
    frame.return_address = return_address;           // ����� ��������

    // ������������� ����������
    for (int i = 0; i < param_count && i < 4; i++) {
        frame.parameters[i] = params[i];
    }

    // ������������� ��������� ���������� (��������)
    for (int i = 0; i < 8; i++) {
        frame.local_variables[i] = 0;
    }

    // ��������� ���� � ����
    stack->frames[++stack->top] = frame;
}

// �������� ����� �� ����� ������� (���������� �������)
StackFrame popFrame(CallStack* stack) {
    if (isEmpty(stack)) {
        printf("������: ���� ������� ����!\n");
        StackFrame error_frame = { "<empty>", -1, {0}, {0} };
        return error_frame;
    }
    return stack->frames[stack->top--];
}

// �������� �������� ����� ����� �������
StackFrame peekFrame(CallStack* stack) {
    if (isEmpty(stack)) {
        printf("������: ���� ������� ����!\n");
        StackFrame error_frame = { "<empty>", -1, {0}, {0} };
        return error_frame;
    }
    return stack->frames[stack->top];
}
