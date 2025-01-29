#pragma once


#include "Instruction.h"

#define NUM_REGISTERS 8      // ���������� ��������� ������ ����������
#define MEMORY_SIZE 1024     // ������ ������ (� ������)

// ����������� ������
typedef struct {
    int32_t registers[NUM_REGISTERS]; // ��������
    uint8_t memory[MEMORY_SIZE];     // ������
    int pc;                          // ������� ������
    int sp;                          // ��������� �����
    int32_t flags;                   // ������� ������ (Zero, Negative)
} VM;

static uint8_t usedRegisters = 0;

void initVM(VM* vm); // ������������� ����������� ������
void setFlags(VM* vm, int32_t result); // ��������������� �������: ��������� ������
void executeInstruction(VM* vm, Instruction* instr); // ���������� ����������
void runVM(VM* vm, Instruction* program, int programSize); // ������������� ����������� ������
void loadProgram(Instruction* program); // ������ ��������� ��� ��
int32_t allocateRegister();
void freeRegister();