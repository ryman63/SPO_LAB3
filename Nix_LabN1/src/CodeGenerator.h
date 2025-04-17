#pragma once

#include "Instruction.h"
#include "ErrorCollector.h"

void generateBuiltIn(ProgramUnit* unit, FILE* fileDescriptor);
void generate(Module* module, FILE* fileDescriptor);
void generateExpr(ExprContext* ctx, FILE* fileDescriptor);
void generateData(ConstData* data, FILE* fileDescriptor);
void addComment(char* message, FILE* fileDescriptor);
void addSection(char* sectionName, FILE* fileDescriptor);
void addMark(char* mark, FILE* fileDescriptor);
char* parseInstructionInLinearCode(Instruction* instr);
char* parseDataInLinearCode(ConstData* data);
char* getRegisterName(enum reg reg);