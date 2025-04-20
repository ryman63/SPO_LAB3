#include "CodeGenerator.h"

void generateBuiltIn(ProgramUnit* unit, FILE* fileDescriptor) {
    addComment("BuiltInFunc", fileDescriptor);
    char* srcCodeFunc = getBuiltInFuncCode(unit->sourceFile, unit->funcSignature->name, fileDescriptor);
    fwrite(srcCodeFunc, strlen(srcCodeFunc), 1, fileDescriptor);
    free(srcCodeFunc);
}

void generate(Module* module, FILE* fileDescriptor) {
    size_t instructionCounter = 1;

    addMark(module->unit->funcSignature->name, fileDescriptor);

    if (module->prologue->size > 0) {
    
        addComment("prologue", fileDescriptor);
        for (size_t i = 0; i < module->prologue->size; i++) {
            Instruction* instr = getItem(module->prologue, i);
            char* parsedInstr = parseInstructionInLinearCode(instr, instructionCounter);
            if (parsedInstr) {
                fwrite(parsedInstr, strlen(parsedInstr), 1, fileDescriptor);
                free(parsedInstr);
                instructionCounter++;
            }
        }
    }

    Array* constDataArray = buildArray(sizeof(ConstData), INITIAL_COUNT_CONST_DATA);

    if (module->exprContextList) {

        addComment("expressions", fileDescriptor);
        for (size_t i = 0; i < module->exprContextList->size; i++) {
            ExprContext* ctx = getItem(module->exprContextList, i);
            generateExpr(ctx, fileDescriptor, &instructionCounter);
            for (size_t j = 0; j < ctx->constDataList->size; j++) {
                ConstData* data = getItem(ctx->constDataList, j);
                pushBack(constDataArray, data);
            }
        }
    }
    

    if (module->prologue->size > 0) {

        addComment("epilogue", fileDescriptor);
        for (size_t i = 0; i < module->epilogue->size; i++) {
            Instruction* instr = getItem(module->epilogue, i);
            char* parsedInstr = parseInstructionInLinearCode(instr, instructionCounter++);
            if (parsedInstr) {
                fwrite(parsedInstr, strlen(parsedInstr), 1, fileDescriptor);

                free(parsedInstr);
                instructionCounter++;
            }
        }
        fwrite("\n", 1, 1, fileDescriptor);
    }

    // генерируем константы
    for (size_t i = 0; i < constDataArray->size; i++) {
        ConstData* data = getItem(constDataArray, i);
        generateData(data, fileDescriptor);
    }
}

void generateExpr(ExprContext* ctx, FILE* fileDescriptor, size_t* instructionNumber) {
    for (size_t i = 0; i < ctx->instructions->size; i++) {
        Instruction* instr = getItem(ctx->instructions, i);
        char* parsedInstr = parseInstructionInLinearCode(instr, *instructionNumber);
        if (parsedInstr) {
            fwrite(parsedInstr, strlen(parsedInstr), 1, fileDescriptor);
            free(parsedInstr);
            (*instructionNumber)++;
        }
    }
}

void generateData(ConstData* data, FILE* fileDescriptor) {
    char* parsedData = parseDataInLinearCode(data);
    fwrite(parsedData, strlen(parsedData), 1, fileDescriptor);
    free(parsedData);
}

void addComment(char* message, FILE* fileDescriptor) {
    size_t buffSize = 16;
    char* buffer = malloc(sizeof(char) * buffSize);
    snprintf(buffer, buffSize, ";%s\n", message);
    fwrite(buffer, strlen(buffer), 1, fileDescriptor);
    free(buffer);
}

void addSection(char* sectionName, FILE* fileDescriptor) {
    size_t buffSize = 16;
    char* buffer = malloc(buffSize);
    snprintf(buffer, buffSize, "[section %s]\n", sectionName);
    fwrite(buffer, strlen(buffer), 1, fileDescriptor);
    free(buffer);
}

void addMark(char* mark, FILE* fileDescriptor)
{
    size_t buffSize = 16;
    char* buffer = malloc(buffSize);
    snprintf(buffer, buffSize, "\n%s:\n", mark);
    fwrite(buffer, strlen(buffer), 1, fileDescriptor);
    free(buffer);
}

char* parseInstructionInLinearCode(Instruction* instr, size_t instructionNumber)
{
    char* resultString = (char*)malloc(INSTRUCTION_MAX_SIZE);
    if (!resultString) {
        printf("Malloc error!\n");
        return NULL;
    }

    char* dest = getRegisterName(instr->dest);
    char* src1 = getRegisterName(instr->src1);
    char* src2 = getRegisterName(instr->src2);
    int offset = abs(instr->offset);
    char* imm = instr->imm;

    switch (instr->opcode) {
    case OC_NOP:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "nop");
        break;

    case OC_LOAD_OFF_MINUS:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "load %s, [%s - %d]", dest, src1, offset);
        break;

    case OC_LOAD_OFF_PLUS:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "load %s, [%s + %d]", dest, src1, offset);
        break;

    case OC_LOAD_LABEL:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "load %s, [%s]", dest, imm);
        break;

    case OC_LOAD_LABEL_OFF:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "load %s, [%s + %d]", dest, imm, offset);
        break;

    case OC_LOAD_REG:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "load %s, [%s]", dest, src1);
        break;

    case OC_LOAD_OFF_MINUS_CONST:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "load %s, const[%s - %d]", dest, src1, offset);
        break;

    case OC_LOAD_OFF_PLUS_CONST:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "load %s, const[%s + %d]", dest, src1, offset);
        break;

    case OC_LOAD_LABEL_CONST:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "load %s, const[%s]", dest, imm);
        break;

    case OC_LOAD_LABEL_OFF_CONST:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "load %s, const[%s + %d]", dest, imm, offset);
        break;

    case OC_LOAD_REG_CONST:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "load %s, const[%s]", dest, src1);
        break;

    case OC_STORE:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "store %s, %s", src1, instr->imm);
        break;

    case OC_STORE_OFF_PLUS:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "store %s, [%s + %d]", src1, dest, offset);
        break;

    case OC_STORE_OFF_MINUS:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "store %s, [%s - %d]", src1, dest, offset);
        break;

    case OC_MOV:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "mov %s, %s", dest, src1);
        break;

    case OC_MOVI:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "movi %s, %s", dest, instr->imm);
        break;

    case OC_IMOV:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "imov %s, %s", dest, instr->imm);
        break;

    case OC_ADD:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "add %s, %s, %s", dest, src1, src2);
        break;

    case OC_SUB:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "sub %s, %s, %s", dest, src1, src2);
        break;

    case OC_SUBI:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "subi %s, %s, %s", dest, src1, instr->imm);
        break;

    case OC_JMP:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "jmp %s", instr->imm);
        break;

    case OC_JZ:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "jz %s", instr->imm);
        break;

    case OC_IN:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "in %s", dest);
        break;

    case OC_OUT:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "out %s", dest);
        break;

    case OC_PUSH:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "push %s", src1);
        break;

    case OC_POP:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "pop %s", dest);
        break;

    case OC_RET:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "ret");
        break;

    case OC_CALL:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "call %s", instr->imm);
        break;

    case OC_HALT:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "hlt");
        break;

    case OC_MARK:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "%s:", instr->imm);
        break;
    default:
        // обработка ошибки
        char buff[16];
        _itoa_s(instr->opcode, buff, 16, 10);
        collectError(ST_GENERATE, "unknown instruction opcode", buff, instructionNumber);

        free(resultString);
        return NULL;

    }

    strcat_s(resultString, INSTRUCTION_MAX_SIZE, "\n");

    return resultString;
}

char* parseDataInLinearCode(ConstData* data)
{
    char* resultString = (char*)malloc(CONST_DATA_MAX_SIZE);
    if (!resultString) {
        printf("Malloc error!\n");
        return NULL;
    }

    char dataSize[4];
    switch (data->dataSize)
    {
    case CD_DB: {
        snprintf(dataSize, 4, "db");
    } break;
    case CD_DD: {
        snprintf(dataSize, 4, "dd");
    } break;
    case CD_DQ: {
        snprintf(dataSize, 4, "dq");
    } break;
    case CD_DW: {
        snprintf(dataSize, 4, "dw");
    } break;
    default:
        break;
    }

    if (data->isRepeat) {
        snprintf(resultString, CONST_DATA_MAX_SIZE, "%s: times %d %s %s", data->mark, data->countRepeat, dataSize, data->value);
    }
    else {
        snprintf(resultString, CONST_DATA_MAX_SIZE, "%s: %s %s", data->mark, dataSize, data->value);
    }

    strcat_s(resultString, CONST_DATA_MAX_SIZE, "\n");

    return resultString;
}

char* getRegisterName(reg reg) {
    switch (reg)
    {
    case gp0: return "gp0";
        break;
    case gp1: return "gp1";
        break;
    case gp2: return "gp2";
        break;
    case gp3: return "gp3";
        break;
    case gp4: return "gp4";
        break;
    case gp5: return "gp5";
        break;
    case gp6: return "gp6";
        break;
    case gp7: return "gp7";
        break;
    case sp: return "sp";
        break;
    case bp: return "bp";
        break;
    case ip: return "IP";
        break;
    case tmp: return "tmp";
        break;
    default: return NULL;
    }
}