#include "constants.h"
#ifndef OPCODE_TABLE_H
#define OPCODE_TABLE_H

typedef struct {
    char mnemonic[MAX_OPCODE_LENGTH + 1];
    int opcode_index;
}   OpcodeTableEntry;
const OpcodeTableEntry* opcode_table_lookup(const char* opcode_name);
const OpcodeTableEntry* get_opcode(const char* line, int skip_chars);
#endif /* OPCODE_TABLE_H */