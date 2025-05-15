#pragma once
#include "Array.h"
#include "Variable.h"
#include "Instruction.h"
#include "StringPool.h"

#define CND_MARK_SIZE 16
#define NUM_REGISTERS 8      //  оличество регистров общего назначени€
#define REG_SIZE 4      //  оличество регистров общего назначени€
#define NUM_LOCAL_VARS 64      // ћакс кол-во локальных переменных
#define INITIAL_COUNT_EXPRS 8      // Ќачальное кол-во выражений в модуле дл€ инициализации массива
#define INITIAL_COUNT_INSTR 4     // Ќачальное кол-во инструкций в контексте выражени€ дл€ инициализации массива
#define INITIAL_COUNT_CONST_DATA 4     // Ќачальное кол-во констант в контексте выражени€ дл€ инициализации массива

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

typedef struct {
	int offset;
	RegisterAllocator* allocator;
	Variable localVars[NUM_LOCAL_VARS];
	size_t countLocalVars;
	MarkGenerator* markGen;
	char* cfgNodeMarks[MAX_NODES];
} MachineState;

typedef struct {
	char* mark;
	Array* instructions;
	Array* constDataList;
	MachineState* state;
	int uncondId;
} ExprContext;

typedef struct {
	ProgramUnit* unit;
	Array* prologue;
	Array* epilogue;
	Array* exprContextList;
} Module;

Module* createModule(ProgramUnit* unit);
ExprContext* createExprContext(const char* mark, int uncondId);
MachineState* createMachineState(MarkGenerator* markGen);
RegisterAllocator* createRegAllocator();
void initRegAllocator(RegisterAllocator* allocator);
MarkGenerator* createMarkGen();

void createVariable(MachineState* module, const char* name, ValueType type);
Variable* findVariable(char* name, MachineState* state);

enum reg allocateRegister(RegisterAllocator* allocator);
void freeRegister(RegisterAllocator* allocator, enum reg reg);


char* getStringMark(MarkGenerator* markGen, char* str, bool* isRepeat);
char* getConditionMark(MarkGenerator* markGen);