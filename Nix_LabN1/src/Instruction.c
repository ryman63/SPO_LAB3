#include "Instruction.h"

Instruction* createInstruction(Opcode opCode, uint8_t dest, uint8_t src1, uint8_t src2, char* imm) {
    Instruction* instr = malloc(sizeof(Instruction));
    instr->opcode = opCode;
    instr->dest = dest;
    instr->src1 = src1;
    instr->src2 = src2;
    instr->imm = imm;
    instr->offset = 0;

    return instr;
}

Instruction* createInstructionWithOffset(Opcode opCode, uint8_t dest, uint8_t src1, uint8_t src2, char* imm, int offset) {
    Instruction* instr = malloc(sizeof(Instruction));
    instr->opcode = opCode;
    instr->dest = dest;
    instr->src1 = src1;
    instr->src2 = src2;
    instr->imm = imm;
    instr->offset = offset;

    return instr;
}

ConstData* createRepeatConstData(char* mark, enum ConstDataSize dataSize, char* value, size_t countRepeat)
{
    ConstData* data = malloc(sizeof(ConstData));
    data->mark = mark;
    data->dataSize = dataSize;
    data->value = value;
    data->isRepeat = true;
    data->countRepeat = countRepeat;

    return data;
}

ConstData* createConstData(char* mark, enum ConstDataSize dataSize, char* value)
{
    ConstData* data = malloc(sizeof(ConstData));
    data->mark = mark;
    data->dataSize = dataSize;
    data->value = value;
    data->isRepeat = false;
    data->countRepeat = 0;

    return data;
}

void I_LOAD_REG(enum reg dest, enum reg src, Array* instrArray) {
    Instruction* instr = createInstruction(OC_LOAD_REG, dest, src, 0, NULL);

    pushBack(instrArray, instr);
}

void I_LOAD_OFF_MINUS(enum reg dest, enum reg src, int offset, Array* instrArray)
{

    Instruction* instr = createInstructionWithOffset(OC_LOAD_OFF_MINUS, dest, src, 0, NULL, offset);

    pushBack(instrArray, instr);
}

void I_LOAD_OFF_PLUS(enum reg dest, enum reg src, int offset, Array* instrArray)
{
    Instruction* instr = createInstructionWithOffset(OC_LOAD_OFF_PLUS, dest, src, 0, NULL, offset);

    pushBack(instrArray, instr);
}

void I_LOAD_LABEL_OFF(enum reg dest, char* address, int offset, Array* instrArray)
{
    Instruction* instr = createInstructionWithOffset(OC_LOAD_LABEL_OFF, dest, 0, 0, address, offset);

    pushBack(instrArray, instr);
}

void I_LOAD_LABEL(enum reg dest, char* address, Array* instrArray)
{
    Instruction* instr = createInstruction(OC_LOAD_LABEL_OFF, dest, 0, 0, address);

    pushBack(instrArray, instr);
}


void I_LOAD_OFF_PLUS_CONST(enum reg dest, enum reg src, int offset, Array* instrArray)
{
    Instruction* instr = createInstructionWithOffset(OC_LOAD_OFF_PLUS_CONST, dest, src, 0, NULL, offset);

    pushBack(instrArray, instr);
}

void I_LOAD_OFF_MINUS_CONST(enum reg dest, enum reg src, int offset, Array* instrArray)
{
    Instruction* instr = createInstructionWithOffset(OC_LOAD_OFF_MINUS_CONST, dest, src, 0, NULL, offset);

    pushBack(instrArray, instr);
}

void I_LOAD_REG_CONST(enum reg dest, enum reg src, Array* instrArray)
{
    Instruction* instr = createInstruction(OC_LOAD_REG_CONST, dest, src, 0, NULL);

    pushBack(instrArray, instr);
}

void I_LOAD_LABEL_CONST(enum reg dest, char* address, Array* instrArray)
{
    Instruction* instr = createInstruction(OC_LOAD_LABEL_CONST, dest, 0, 0, address);

    pushBack(instrArray, instr);
}

void I_LOAD_LABEL_OFF_CONST(enum reg dest, char* address, int offset, Array* instrArray)
{
    Instruction* instr = createInstructionWithOffset(OC_LOAD_LABEL_OFF_CONST, dest, 0, 0, address, offset);

    pushBack(instrArray, instr);
}


void I_STORE(enum reg src, int address, Array* instrArray) {
    char* buffAddress = malloc(sizeof(char) * 16);

    _itoa_s(address, buffAddress, 16, 10);

    Instruction* instr = createInstruction(OC_STORE, 0, src, 0, buffAddress);

    pushBack(instrArray, instr);
}

void I_STORE_OFF_PLUS(enum reg src, enum reg dest, int offset, Array* instrArray)
{
    Instruction* instr = createInstructionWithOffset(OC_STORE_OFF_PLUS, dest, src, 0, NULL, offset);

    pushBack(instrArray, instr);
}

void I_STORE_OFF_MINUS(enum reg src, enum reg dest, int offset, Array* instrArray)
{
    Instruction* instr = createInstructionWithOffset(OC_STORE_OFF_MINUS, dest, src, 0, NULL, offset);

    pushBack(instrArray, instr);
}

void I_MOV(enum reg dest, enum reg src, Array* instrArray) {
    Instruction* instr = createInstruction(OC_MOV, dest, src, 0, NULL);

    pushBack(instrArray, instr);
}

void I_MOVI(enum reg dest, char* immediate, Array* instrArray) {

    Instruction* instr = createInstruction(OC_MOVI, dest, 0, 0, immediate);

    pushBack(instrArray, instr);
}

void I_CONST_INT(char* mark, enum ConstDataSize dataSize, int value, Array* dataArray)
{
    char* buffImmediate = malloc(sizeof(char) * 16);

    _itoa_s(value, buffImmediate, 16, 10);

    createConstData(strCpy(mark), dataSize, buffImmediate);
}

void I_CONST_STR(char* mark, char* value, Array* dataArray) {
    const size_t buffSize = 4;
    char* valueCpy = strCpy(value);
    strncat_s(valueCpy, strlen(valueCpy) + buffSize, ", 0", _TRUNCATE);

    
    ConstData* data = createConstData(mark, CD_DB, valueCpy);
    pushBack(dataArray, data);
}

void I_SUBI(enum reg dest, enum reg src1, int src2, Array* instrArray) {
    char* buffImmediate = malloc(sizeof(char) * 16);

    _itoa_s(src2, buffImmediate, 16, 10);

    Instruction* instr = createInstruction(OC_SUBI, dest, src1, 0, buffImmediate);
    pushBack(instrArray, instr);
}

void I_SUB(enum reg dest, enum reg src1, enum reg src2, Array* instrArray) {
    Instruction* instr = createInstruction(OC_SUB, dest, src1, src2, NULL);
    pushBack(instrArray, instr);
}

void I_ADD(enum reg dest, enum reg src1, enum reg src2, Array* instrArray) {
    Instruction* instr = createInstruction(OC_ADD, dest, src1, src2, NULL);
    pushBack(instrArray, instr);
}

void I_CMP(enum reg src1, enum reg src2, Array* instrArray) {
    Instruction* instr = createInstruction(OC_CMP, 0, src1, src2, NULL);
    pushBack(instrArray, instr);
}

void I_JMP(char* mark, Array* instrArray) {
    Instruction* instr = createInstruction(OC_JMP, 0, 0, 0, mark);
    pushBack(instrArray, instr);
}

void I_JE(char* mark, Array* instrArray) {
    Instruction* instr = createInstruction(OC_JE, 0, 0, 0, strCpy(mark));
    pushBack(instrArray, instr);
}

void I_JNE(char* mark, Array* instrArray) {
    Instruction* instr = createInstruction(OC_JNE, 0, 0, 0, strCpy(mark));
    pushBack(instrArray, instr);
}

void I_JL(char* mark, Array* instrArray) {
    Instruction* instr = createInstruction(OC_JL, 0, 0, 0, strCpy(mark));
    pushBack(instrArray, instr);
}

void I_JLE(char* mark, Array* instrArray) {
    Instruction* instr = createInstruction(OC_JLE, 0, 0, 0, strCpy(mark));
    pushBack(instrArray, instr);
}

void I_JG(char* mark, Array* instrArray) {
    Instruction* instr = createInstruction(OC_JG, 0, 0, 0, strCpy(mark));
    pushBack(instrArray, instr);
}

void I_JGE(char* mark, Array* instrArray) {
    Instruction* instr = createInstruction(OC_JGE, 0, 0, 0, strCpy(mark));
    pushBack(instrArray, instr);
}

void I_IN(enum reg dest, Array* instrArray) {
    Instruction* instr = createInstruction(OC_IN, dest, 0, 0, NULL);
    pushBack(instrArray, instr);
}

void I_OUT(enum reg src, Array* instrArray) {
    Instruction* instr = createInstruction(OC_OUT, 0, src, 0, NULL);
    pushBack(instrArray, instr);
}

void I_PUSH(enum reg src, Array* instrArray) {
    Instruction* instr = createInstruction(OC_PUSH, 0, src, 0, NULL);
    pushBack(instrArray, instr);
}

void I_POP(enum reg dest, Array* instrArray) {
    Instruction* instr = createInstruction(OC_POP, dest, 0, 0, NULL);
    pushBack(instrArray, instr);
}

void I_CALL(char* mark, Array* instrArray) {
    Instruction* instr = createInstruction(OC_CALL, 0, 0, 0, mark);
    pushBack(instrArray, instr);
}

void I_RET(Array* instrArray) {
    Instruction* instr = createInstruction(OC_RET, 0, 0, 0, NULL);
    pushBack(instrArray, instr);
}

void I_HALT(Array* instrArray)
{
    Instruction* instr = createInstruction(OC_HALT, 0, 0, 0, NULL);
    pushBack(instrArray, instr);
}

void I_MARK(Array* instrArray, char* mark) {
    char* markCpy = strCpy(mark);
    Instruction* instr = createInstruction(OC_MARK, 0, 0, 0, markCpy);
    pushBack(instrArray, instr);
}

