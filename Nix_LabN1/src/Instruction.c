#include "Instruction.h"

BasicBlock* createBasicBlock() {
    BasicBlock* block = malloc(sizeof(BasicBlock));
    block->id = block_idCounter;
    block->instructions = NULL;

    block_idCounter++;
    return block;
}

void addInstruction(BasicBlock* block, Instruction* instruction) {
    if (!block->instructions) {
        block->instructions = instruction;
    }
    else {
        Instruction* current = block->instructions->next;
        while (current) {
            current = current->next;
        }
        current->next = instruction;
    }
}


Instruction* createInstruction(Opcode opCode, uint8_t dest, uint8_t src1, uint8_t src2, int32_t imm, Instruction* next) {
    Instruction* instr = malloc(sizeof(Instruction));
    instr->opcode = opCode;
    instr->dest = dest;
    instr->src1 = src1;
    instr->src2 = src2;
    instr->imm = imm;
    instr->next = next;

    return instr;
}
