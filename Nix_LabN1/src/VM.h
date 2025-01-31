#pragma once


#include "Instruction.h"

#define NUM_REGISTERS 8      // Количество регистров общего назначения
#define MEMORY_SIZE 1024     // Размер памяти (в байтах)

// Виртуальная машина
typedef struct {
    int32_t registers[NUM_REGISTERS]; // Регистры
    uint8_t memory[MEMORY_SIZE];     // Память
    int pc;                          // Счётчик команд
    int sp;                          // Указатель стека
    int32_t flags;                   // Регистр флагов (Zero, Negative)
} VM;

static uint8_t usedRegisters = 0;

void initVM(VM* vm); // Инициализация виртуальной машины
void setFlags(VM* vm, int32_t result); // Вспомогательная функция: установка флагов
void loadProgram(Instruction* program, BasicBlock* blocks, size_t countBlocks);
void runVM(VM* vm, Instruction* program);
int32_t allocateRegister();
void freeRegister();
char* parseInstructionInLinearCode(VM* vm, Instruction* instr);