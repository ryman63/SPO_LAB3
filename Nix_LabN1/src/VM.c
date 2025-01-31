#include "VM.h"


void initVM(VM* vm) {
    memset(vm->registers, 0, sizeof(vm->registers));
    memset(vm->memory, 0, sizeof(vm->memory));
    vm->pc = 0;
    vm->sp = MEMORY_SIZE - 1;
    vm->flags = 0;
}

void setFlags(VM* vm, int32_t result) {
    vm->flags = 0;
    if (result == 0) vm->flags |= 1;  // Zero flag
    if (result < 0) vm->flags |= 2;   // Negative flag
}


void runVM(VM* vm, Instruction* program) {

    FILE* asmListing = fopen("program.asm", "w");

    for (size_t i = 0; i < (size_t)vm->pc; i++) {
        Instruction* instr = &program[vm->pc];
        char* linearCode = parseInstructionInLinearCode(instr, vm);
        strcat_s(linearCode, INSTRUCTION_MAX_SIZE, "\n");
        fwrite(linearCode, sizeof(char), INSTRUCTION_MAX_SIZE, asmListing);
        free(linearCode);
    }

    fclose(asmListing);
}

void loadProgram(Instruction* program, BasicBlock* blocks, size_t countBlocks) {

    for (size_t i = 0; i < countBlocks; i++) {
        BasicBlock currentBlock = blocks[i];
        Instruction* currentInstruction = currentBlock.instructions;
        size_t instructionCounter = 0;
        while (currentInstruction) {
            program[instructionCounter] = *currentInstruction;
            currentInstruction = currentInstruction->next;
        }
    }
}

int32_t allocateRegister()
{
    if (usedRegisters < NUM_REGISTERS) {
        return usedRegisters++;
    }
    else {
        // нет доступных регистров
    }
}

void freeRegister()
{
    if (usedRegisters > 0) {
        usedRegisters--;
    }
    else {
        // нет используемых регистров
    }
}

char* parseInstructionInLinearCode(VM* vm, Instruction* instr)
{
    char* resultString = (char*)malloc(64);
    if (!resultString) {
        printf("Ошибка выделения памяти!\n");
        return NULL;
    }

    switch (instr->opcode) {
    case OC_NOP:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "NOP");
        break;

    case OC_LOAD:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "LOAD R%d, %s", instr->dest, instr->imm);
        break;

    case OC_STORE:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "STORE R%d, %s", instr->dest, instr->imm);
        break;

    case OC_MOV:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "MOV R%d, R%d", instr->dest, instr->src1);
        break;

    case OC_ADD:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "ADD R%d, R%d, R%d", instr->dest, instr->src1, instr->src2);
        break;

    case OC_SUB:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "SUB R%d, R%d, R%d", instr->dest, instr->src1, instr->src2);
        break;

    case OC_JMP:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "JMP %s", instr->imm);
        break;

    case OC_JZ:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "JZ R%d, %s", instr->src1, instr->imm);
        break;

    case OC_IN:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "IN R%d", instr->dest);
        break;

    case OC_OUT:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "OUT R%d", instr->dest);
        break;

    case OC_RET:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "RET");
        break;

    case OC_CALL:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "CALL %s", instr->imm);
        break;

    case OC_HALT:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "HALT");
        break;

    case OC_MARK:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "%s:", instr->marker);
    default:
        // обработка ошибки
        break;
    }

    vm->pc += 1;

    return resultString;
}
