#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "constants.h"

typedef struct {
    int opcode;
    char operands[MAX_OPERANDS_LENGTH + 1];
} Instruction;

#endif /* INSTRUCTION_H */