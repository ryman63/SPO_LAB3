#pragma once
#include "Array.h"
#include "Variable.h"
#include "Instruction.h"
#include "StringPool.h"

#define CND_MARK_SIZE 16
#define NUM_REGISTERS 8      // ���������� ��������� ������ ����������
#define REG_SIZE 4      // ���������� ��������� ������ ����������
#define NUM_LOCAL_VARS 64      // ���� ���-�� ��������� ����������
#define INITIAL_COUNT_EXPRS 8      // ��������� ���-�� ��������� � ������ ��� ������������� �������
#define INITIAL_COUNT_INSTR 4     // ��������� ���-�� ���������� � ��������� ��������� ��� ������������� �������
#define INITIAL_COUNT_CONST_DATA 4     // ��������� ���-�� �������� � ��������� ��������� ��� ������������� �������

typedef enum reg {
	gp0, gp1, gp2, gp3, gp4, gp5, gp6, gp7,
	sp, bp, ip, tmp
} reg;


typedef struct RegisterAllocator {
	bool used[NUM_REGISTERS];
} RegisterAllocator;

typedef struct MarkGenerator {
	size_t countConditions;
	StringPool* strPool;
} MarkGenerator;

typedef struct MachineState{
	int offset;
	RegisterAllocator* allocator;
	Variable localVars[NUM_LOCAL_VARS];
	size_t countLocalVars;
	MarkGenerator* markGen;
	char* cfgNodeMarks[MAX_NODES];
} MachineState;

typedef struct ExprContext{
	char* mark;
	Array* instructions;
	Array* constDataList;
	MachineState* state;
	int condId;
} ExprContext;

typedef struct Module{
	ProgramUnit* unit;
	Array* prologue;
	Array* epilogue;
	Array* exprContextList;
} Module;

Module* createModule(ProgramUnit* unit);
ExprContext* createExprContext(const char* mark, int condId);
MachineState* createMachineState(MarkGenerator* markGen);
RegisterAllocator* createRegAllocator();
void initRegAllocator(RegisterAllocator* allocator);
MarkGenerator* createMarkGen();

size_t createVariable(MachineState* module, const char* name, ValueType* valueType);
Variable* findVariable(char* name, MachineState* state);

enum reg allocateRegister(RegisterAllocator* allocator);
void freeRegister(RegisterAllocator* allocator, enum reg reg);


char* getStringMark(MarkGenerator* markGen, char* str, bool* isRepeat);
char* getConditionMark(MarkGenerator* markGen);