#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "symbol_table.h"
#include "constants.h"
#include "utilities.h"
void add_ic_to_all_data(int ic)
{
    SymbolTableEntry* current_entry = global_state.symbol_table;
    while (current_entry->next) {
        if (current_entry->symbol_type == DATA_DIRECTIVE)
            current_entry->address += ic;
        current_entry = current_entry->next;
    }
}
bool add_to_symbol_table(const char* symbol, unsigned int address,
    bool relocatable, bool data_part,char* symbol_type) {
    SymbolTableEntry* new_entry = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
    if (!new_entry) {
        return false;
    }

    strncpy(new_entry->symbol, symbol, MAX_SYMBOL_LENGTH);
    new_entry->address = address;
    new_entry->relocatable = relocatable;
    new_entry->data_part = data_part;
    new_entry->symbol_type = symbol_type;

    new_entry->next = NULL;

    if (!global_state.symbol_table) {
        global_state.symbol_table = new_entry;
    }
    else {
        SymbolTableEntry* current_entry = global_state.symbol_table;
        while (current_entry->next) {
            current_entry = current_entry->next;
        }
        current_entry->next = new_entry;
    }
    return true;
}
/* Find the symbol table entry with the given symbol name */
SymbolTableEntry* get_symbol(const char* symbol_name) {
    // Traverse the linked list of symbol table entries
    SymbolTableEntry* entry = global_state.symbol_table;
    while (entry != NULL) {
        // Compare the symbol name with the entry's symbol
        if (strcmp(entry->symbol, symbol_name) == 0) {
            return entry; // return the matching entry
        }
        entry = entry->next; // move to the next entry in the linked list
    }
    // No matching symbol was found in the symbol table
    return NULL;
}
bool is_valid_label(const char* label) {
    SymbolTableEntry* entry = get_symbol(label);
    if (entry == NULL) {
        return false;
    }
    return true;
}
void process_data_directive(const char* line) {
    char label[MAX_SYMBOL_LENGTH + 1] = { 0 };
    sscanf(line, "%s .data", label);

    if (strlen(label) > 0) {
        add_to_symbol_table(label, global_state.data_counter,
            true, true,DATA_DIRECTIVE);
    }

    const char* data_start = strstr(line, ".data") + strlen(".data");
    int value;
    while (sscanf(data_start, "%d", &value) == 1) {
        global_state.data_counter++;
        data_start = strchr(data_start, ',') + 1;
    }
}