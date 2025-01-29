#pragma once
#include <stdint.h>
#include <stdlib.h>

#include <stdio.h>
#include <string.h>

typedef enum Opcode {
    OC_NOP = 0, OC_LOAD, OC_STORE, OC_MOV, OC_ADD, OC_SUB, OC_JMP, OC_JZ, OC_IN, OC_OUT, OC_PUSH, OC_POP, OC_CALL, OC_RET
} Opcode;

typedef struct Instruction {
    enum Opcode opcode;
    uint8_t dest;
    uint8_t src1;
    uint8_t src2;
    int32_t imm;
    struct Instruction* next;
} Instruction;

static int block_idCounter = 0;

typedef struct BasicBlock {
    int id;
    struct Instruction* instructions;
} BasicBlock;

BasicBlock* createBasicBlock();

void addInstruction(BasicBlock* block, Instruction* instruction);

Instruction* createInstruction(Opcode opCode, uint8_t dest, uint8_t src1, uint8_t src2, int32_t imm, Instruction* next);