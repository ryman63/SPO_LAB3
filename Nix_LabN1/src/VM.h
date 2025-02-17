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
void loadProgram(Instruction* program, BasicBlock** blocks, size_t countBlocks);
void runVM(VM* vm, Instruction* program, size_t instrCount);
int32_t allocateRegister();
void freeRegister();
char* parseInstructionInLinearCode(VM* vm, Instruction* instr);
char* getRegisterName(enum reg reg);

typedef enum reg{
    gp0, gp1, gp2, gp3, gp4, gp5, gp6, gp7,
    sp, rag,
} reg;