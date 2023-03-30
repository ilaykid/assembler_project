#include "constants.h"
#include <stdbool.h>
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
typedef struct SymbolTableEntry {
    char symbol[MAX_SYMBOL_LENGTH + 1];
    unsigned int address;
    bool relocatable;
    bool data_part;
    char* symbol_type;
    struct SymbolTableEntry* next;
} SymbolTableEntry;
SymbolTableEntry* get_symbol(const char* symbol_name);
bool is_valid_label(const char* label);
bool add_to_symbol_table(const char* symbol, unsigned int address,
    bool relocatable, bool data_part, char* symbol_type);
void add_ic_to_all_data(int ic);
#endif /* SYMBOL_TABLE_H */
