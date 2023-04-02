#include "constants.h"
#ifndef OPCODE_TABLE_H
#define OPCODE_TABLE_H

typedef struct {
    char mnemonic[MAX_OPCODE_LENGTH + 1];
    int opcode_index;
}   OpcodeTableEntry;
 OpcodeTableEntry* opcode_table_lookup( char* opcode_name);
 OpcodeTableEntry* get_opcode( char* line, int skip_chars);
#endif /* OPCODE_TABLE_H */