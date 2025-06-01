#include "Module.h"

Module* createModule(ProgramUnit* unit) {
	Module* module = malloc(sizeof(Module));
	module->unit = unit;
	module->prologue = buildArray(sizeof(Instruction), INITIAL_COUNT_INSTR);
	module->epilogue = buildArray(sizeof(Instruction), INITIAL_COUNT_INSTR);
	module->exprContextList = buildArray(sizeof(ExprContext), INITIAL_COUNT_EXPRS);
	
	return module;
}

ExprContext* createExprContext(const char* mark, int condId) {
	ExprContext* exprCtx = malloc(sizeof(ExprContext));
	exprCtx->mark = mark;
	exprCtx->instructions = buildArray(sizeof(Instruction), INITIAL_COUNT_INSTR);
	exprCtx->constDataList = buildArray(sizeof(ConstData), INITIAL_COUNT_CONST_DATA);
	exprCtx->condId = condId;

	return exprCtx;
}

MachineState* createMachineState(MarkGenerator* markGen) {
	MachineState* state = malloc(sizeof(MachineState));
	state->offset = 0;
	state->countLocalVars = 0;
	state->allocator = createRegAllocator();
	state->markGen = markGen;
	memset(state->cfgNodeMarks, NULL, sizeof(char));

	return state;
}

RegisterAllocator* createRegAllocator()
{
	RegisterAllocator* allocator = malloc(sizeof(RegisterAllocator));
	initRegAllocator(allocator);
	
	return allocator;
}

size_t createVariable(MachineState* state, const char* name, ValueType* valueType) {
	Variable varParam;
	varParam.address = state->offset;
	varParam.name = strCpy(name);
	varParam.type = valueType;
	varParam.isInit = false;
	state->localVars[state->countLocalVars] = varParam;
	state->countLocalVars++;

	size_t sizeOfType = getSizeOfType(valueType);
	state->offset -= sizeOfType;
	
	return sizeOfType;
}

Variable* findVariable(char* name, MachineState* state) {
	for (size_t i = 0; i < state->countLocalVars; i++) {
		Variable var = state->localVars[i];
		if (strcmp(var.name, name) == 0)
			return &var;
	}

	return NULL;
}

void initRegAllocator(RegisterAllocator* allocator) {
	for (size_t i = 0; i < NUM_REGISTERS; i++) {
		allocator->used[i] = false;
	}
}

MarkGenerator* createMarkGen()
{
	MarkGenerator* markGen = malloc(sizeof(MarkGenerator));
	markGen->strPool = init_pool();
	markGen->countConditions = 0;
}

char* getStringMark(MarkGenerator* markGen, char* str, bool* isRepeat)
{
	char* uniqueMark = intern_string(markGen->strPool, str, isRepeat);
	return uniqueMark;
}

char* getConditionMark(MarkGenerator* markGen) {
	char* uniqueMark = malloc(sizeof(char) * CND_MARK_SIZE);
	snprintf(uniqueMark, CND_MARK_SIZE, ".cnd_%d", markGen->countConditions++);
	return uniqueMark;
}

reg allocateRegister(RegisterAllocator* allocator) {
	for (size_t i = 0; i < NUM_REGISTERS; i++) {
		if (allocator->used[i] == false) {
			allocator->used[i] = true;
			return i;
		}
	}
	return -1;
}

void freeRegister(RegisterAllocator* allocator, enum reg reg) {
	if (reg >= 0 && reg < NUM_REGISTERS) {
		allocator->used[reg] = false;
	}
}