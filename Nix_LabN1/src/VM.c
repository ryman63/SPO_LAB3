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

void executeInstruction(VM* vm, Instruction* instr) {
    switch (instr->opcode) {
    case OC_NOP:
        break;

    case OC_LOAD:
        vm->registers[instr->dest] = instr->imm;
        break;

    case OC_STORE:
        vm->memory[instr->imm] = vm->registers[instr->src1];
        break;

    case OC_MOV:
        vm->registers[instr->dest] = vm->registers[instr->src1];
        break;

    case OC_ADD:
        vm->registers[instr->dest] = vm->registers[instr->src1] + vm->registers[instr->src2];
        setFlags(vm, vm->registers[instr->dest]);
        break;

    case OC_SUB:
        vm->registers[instr->dest] = vm->registers[instr->src1] - vm->registers[instr->src2];
        setFlags(vm, vm->registers[instr->dest]);
        break;

    case OC_JMP:
        vm->pc = instr->imm;
        return;

    case OC_JZ:
        if (vm->flags & 1) {
            vm->pc = instr->imm;
            return;
        }
        break;

    case OC_IN:
        printf("Введите значение для регистра R%d: ", instr->dest);
        //scanf("%d", &vm->registers[instr->dest]);
        break;

    case OC_OUT:
        printf("Вывод R%d: %d\n", instr->src1, vm->registers[instr->src1]);
        break;

    default:
        printf("Неизвестная инструкция!\n");
        exit(1);
    }
    vm->pc++; // Переход к следующей инструкции
}

void runVM(VM* vm, Instruction* program, int programSize) {
    while (vm->pc < programSize) {
        Instruction* instr = &program[vm->pc];
        executeInstruction(vm, instr);
    }
}

void loadProgram(Instruction* program) {
    program[0] = (Instruction){ OC_LOAD, 0, 0, 0, 42 };   // R0 <- 42
    program[1] = (Instruction){ OC_LOAD, 1, 0, 0, 10 };   // R1 <- 10
    program[2] = (Instruction){ OC_ADD, 2, 0, 1, 0 };     // R2 <- R0 + R1
    program[3] = (Instruction){ OC_OUT, 0, 2, 0, 0 };     // Вывод R2
    program[4] = (Instruction){ OC_JMP, 0, 0, 0, 4 };     // Переход на себя (бесконечный цикл)
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
