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
void loadProgram(Instruction* program, BasicBlock* blocks, size_t countBlocks);
void runVM(VM* vm, Instruction* program);
int32_t allocateRegister();
void freeRegister();
char* parseInstructionInLinearCode(VM* vm, Instruction* instr);