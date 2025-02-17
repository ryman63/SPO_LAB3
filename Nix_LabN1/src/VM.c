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


void runVM(VM* vm, Instruction* program, size_t instrCount) {

    FILE* asmListing = fopen("program.asm", "w");

    for (size_t i = 0; i < instrCount; i++) {
        Instruction* instr = &program[i];
        char* linearCode = parseInstructionInLinearCode(vm, instr);
        strcat_s(linearCode, INSTRUCTION_MAX_SIZE, "\n");
        fwrite(linearCode, sizeof(char), INSTRUCTION_MAX_SIZE, asmListing);
        free(linearCode);
    }

    fclose(asmListing);
}

void loadProgram(Instruction* program, BasicBlock** blocks, size_t countBlocks) {
    size_t instructionCounter = 0;
    for (size_t i = 0; i < countBlocks; i++) {
        BasicBlock* currentBlock = blocks[i];
        Instruction* currentInstruction = currentBlock->instructions;
        
        while (currentInstruction) {
            program[instructionCounter] = *currentInstruction;
            currentInstruction = currentInstruction->next;
            instructionCounter++;
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
    char* resultString = (char*)malloc(INSTRUCTION_MAX_SIZE);
    if (!resultString) {
        printf("Ошибка выделения памяти!\n");
        return NULL;
    }

    char* dest = getRegisterName(instr->dest);
    char* src1 = getRegisterName(instr->src1);
    char* src2 = getRegisterName(instr->src2);

    switch (instr->opcode) {
    case OC_NOP:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "NOP");
        break;

    case OC_LOAD:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "load %s, %s", dest, instr->imm);
        break;

    case OC_STORE:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "store %s, %s", dest, instr->imm);
        break;

    case OC_MOV:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "mov %s, %s", dest, src1);
        break;

    case OC_ADD:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "add %s, %s, %s", dest, src1, src2);
        break;

    case OC_SUB:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "sub %s, %s, %s", dest, src1, src2);
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

    case OC_RET:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "ret");
        break;

    case OC_CALL:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "call %s", instr->imm);
        break;

    case OC_HALT:
        snprintf(resultString, INSTRUCTION_MAX_SIZE, "halt");
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
    case rag: return "rar";
        break;
    default: return NULL;
    }
}
