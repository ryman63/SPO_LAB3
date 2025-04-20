#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stdio.h>
#include <string.h>
#include "Module.h"

#define INSTRUCTION_MAX_SIZE 64
#define CONST_DATA_MAX_SIZE 4096

typedef enum Opcode {
    OC_NOP = 0, 
    OC_LOAD_REG, OC_LOAD_LABEL, OC_LOAD_LABEL_OFF, OC_LOAD_OFF_PLUS, OC_LOAD_OFF_MINUS,
    OC_LOAD_REG_CONST, OC_LOAD_LABEL_CONST, OC_LOAD_LABEL_OFF_CONST, OC_LOAD_OFF_PLUS_CONST, OC_LOAD_OFF_MINUS_CONST,
    OC_STORE, OC_STORE_OFF_PLUS, OC_STORE_OFF_MINUS,  
    OC_MOV, OC_MOVI, OC_IMOV, 
    OC_ADD, OC_SUB, OC_SUBI, 
    OC_JMP, OC_JZ, 
    OC_IN, OC_OUT, 
    OC_PUSH, OC_POP, 
    OC_CALL, OC_RET, 
    OC_HALT, 
    OC_MARK
} Opcode;

typedef enum ConstDataSize {
    CD_DB,
    CD_DW,
    CD_DD,
    CD_DQ
} ConstDataSize;

typedef struct Instruction {
    enum Opcode opcode;
    uint8_t dest;
    uint8_t src1;
    uint8_t src2;
    char* imm;
    int offset;
} Instruction;

typedef struct ConstData {
    char* mark;
    char* value;
    enum ConstDataSize dataSize;
    bool isRepeat;
    size_t countRepeat;
} ConstData;

Instruction* createInstruction(Opcode opCode, uint8_t dest, uint8_t src1, uint8_t src2, char* imm);
Instruction* createInstructionWithOffset(Opcode opCode, uint8_t dest, uint8_t src1, uint8_t src2, char* imm, int offset);
ConstData* createConstData(char* mark, enum ConstDataSize dataSize, char* value);
ConstData* createRepeatConstData(char* mark, enum ConstDataSize dataSize, char* value, size_t countRepeat);

void I_LOAD_REG(enum reg dest, enum reg src, Array* instrArray);
void I_LOAD_LABEL(enum reg dest, char* address, Array* instrArray);
void I_LOAD_LABEL_OFF(enum reg dest, char* address, int offset, Array* instrArray);
void I_LOAD_OFF_PLUS(enum reg dest, enum reg src, int offset, Array* instrArray);
void I_LOAD_OFF_MINUS(enum reg dest, enum reg src, int offset, Array* instrArray);
void I_LOAD_REG_CONST(enum reg dest, enum reg src, Array* instrArray);
void I_LOAD_LABEL_CONST(enum reg dest, char* address, Array* instrArray);
void I_LOAD_LABEL_OFF_CONST(enum reg dest, int address, int offset, Array* instrArray);
void I_LOAD_OFF_PLUS_CONST(enum reg dest, enum reg src, int offset, Array* instrArray);
void I_LOAD_OFF_MINUS_CONST(enum reg dest, enum reg src, int offset, Array* instrArray);

void I_STORE(enum reg src, int address, Array* instrArray);
void I_STORE_OFF_PLUS(enum reg src, enum reg dest, int offset, Array* instrArray);
void I_STORE_OFF_MINUS(enum reg src, enum reg dest, int offset, Array* instrArray);
void I_MOV(enum reg dest, enum reg src, Array* instrArray);
void I_MOVI(enum reg dest, char* immediate, Array* instrArray);

void I_CONST_INT(char* mark, enum ConstDataSize dataSize, int value, Array* dataArray);
void I_CONST_STR(char* mark, char* value, Array* dataArray);

void I_SUBI(enum reg dest, enum reg src1, int src2, Array* instrArray);
void I_SUB(enum reg dest, enum reg src1, enum reg src2, Array* instrArray);
void I_ADD(enum reg dest, enum reg src1, enum reg src2, Array* instrArray);
//void SUB(enum reg dest, enum reg src1, enum reg src2, Array* instrArray);
//void SUB(enum reg dest, enum reg src1, enum reg src2, Array* instrArray);

void I_JMP(char* mark, Array* instrArray);
void I_JZ(char* mark, Array* instrArray);

void I_IN(enum reg dest, Array* instrArray);
void I_OUT(enum reg src, Array* instrArray);

void I_PUSH(enum reg src, Array* instrArray);
void I_POP(enum reg dest, Array* instrArray);

void I_CALL(char* mark, Array* instrArray);
void I_RET(Array* instrArray);

void I_HALT(Array* instrArray);

void I_MARK(Array* instrArray, char* mark);