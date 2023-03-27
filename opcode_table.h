#include "constants.h"
#ifndef OPCODE_TABLE_H
#define OPCODE_TABLE_H

typedef struct {
    int opcode;
    char mnemonic[MAX_OPCODE_LENGTH + 1];
    int num_words;
}   OpcodeTableEntry;

#endif /* OPCODE_TABLE_H */