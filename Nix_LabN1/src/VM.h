#pragma once


#include "Instruction.h"

#define NUM_REGISTERS 8      // Количество регистров общего назначения
#define MEMORY_SIZE 1024     // Размер памяти (в байтах)
#define MEMORY_CODE_NAME "code"   // название банки памяти с кодом

typedef struct RegisterAllocator {
    bool used[NUM_REGISTERS];
} RegisterAllocator;

// Виртуальная машина
typedef struct VM {
    int32_t registers[NUM_REGISTERS]; // Регистры
    uint8_t memory[MEMORY_SIZE];     // Память
    int pc;                          // Счётчик команд
    int sp;                          // Указатель стека
    int32_t flags;                   // Регистр флагов (Zero, Negative)
    struct RegisterAllocator allocator;
} VM;

void initVM(VM* vm); // Инициализация виртуальной машины
void setFlags(VM* vm, int32_t result); // Вспомогательная функция: установка флагов
void loadProgram(Instruction* program, BasicBlock** blocks, size_t countBlocks);
void runVM(VM* vm, Instruction* program, size_t instrCount);


char* parseInstructionInLinearCode(VM* vm, Instruction* instr);
char* getRegisterName(enum reg reg);
void addCodeSection(FILE* asmListing);

typedef enum reg{
    gp0, gp1, gp2, gp3, gp4, gp5, gp6, gp7,
    sp, rag, bp,
} reg;