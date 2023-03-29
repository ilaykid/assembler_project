#include "constants.h"
#ifndef OPCODE_TABLE_H
#define OPCODE_TABLE_H

typedef struct {
    char mnemonic[MAX_OPCODE_LENGTH + 1];
    int opcode_index;
    int num_words;
}   OpcodeTableEntry;

#endif /* OPCODE_TABLE_H */