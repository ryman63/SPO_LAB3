#pragma once
#include <stdint.h>
#include <stdlib.h>

#include <stdio.h>
#include <string.h>

typedef enum Opcode {
    OC_NOP = 0, OC_LOAD, OC_STORE, OC_MOV, OC_MOVI, OC_ADD, OC_SUB, OC_JMP, OC_JZ, OC_IN, OC_OUT, OC_PUSH, OC_POP, OC_CALL, OC_RET, OC_HALT, OC_MARK
} Opcode;

#define INSTRUCTION_MAX_SIZE 64

typedef struct Instruction {
    enum Opcode opcode;
    uint8_t dest;
    uint8_t src1;
    uint8_t src2;
    char* imm;
    char* marker;
    struct Instruction* next;
} Instruction;

static int block_idCounter = 0;

typedef struct BasicBlock {
    int id;
    struct Instruction* instructions;
    size_t countInstructions;
} BasicBlock;

BasicBlock* createBasicBlock();

void addInstruction(BasicBlock* block, Instruction* instruction);

Instruction* createInstruction(Opcode opCode, uint8_t dest, uint8_t src1, uint8_t src2, char* imm, char* marker);