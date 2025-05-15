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
void pushFrame(CallStack* stack, ProgramUnit* programUnit) {
    if (isFull(stack)) {
        printf("������: ���� ������� ����������!\n");
        return;
    }

    // ������ ����� ����
    StackFrame frame;

    //frame.return_address = return_address;
    frame.programUnit = programUnit;
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
