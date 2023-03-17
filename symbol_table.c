#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "symbol_table.h"
#include "constants.h"

/* initialize SymbolTable */
SymbolTableEntry* init_symbol_table() {
    SymbolTableEntry* symbol_table = NULL;
    return symbol_table;
}
/* Function to create a new symbol node */
SymbolNode* new_symbol_node(const char* label, int address, SymbolAttribute attribute)
{
    SymbolNode* node = (SymbolNode*)malloc(sizeof(SymbolNode));
    if (!node)
    {
        fprintf(stderr, "Error: Failed to allocate memory for a new symbol node.\n");
        exit(EXIT_FAILURE);
    }

    strncpy(node->label, label, MAX_LABEL_LENGTH);
    node->label[MAX_LABEL_LENGTH - 1] = '\0';
    node->address = address;
    node->attribute = attribute;
    node->next = NULL;

    return node;
}

/* Function to add a symbol to the symbol table */
void add_symbol(SymbolTable* table, const char* label, int address, SymbolAttribute attribute)
{
    SymbolNode* new_node = new_symbol_node(label, address, attribute);

    if (table->head == NULL)
    {
        table->head = new_node;
    }
    else
    {
        SymbolNode* current = table->head;
        while (current->next)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}

/* Function to search for a symbol in the symbol table */
SymbolNode* find_symbol(SymbolTable* table, const char* label)
{
    SymbolNode* current = table->head;

    while (current)
    {
        if (strcmp(current->label, label) == 0)
        {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

/* Function to add a new symbol to the symbol table */
SymbolTableEntry* add_to_symbol_table(SymbolTableEntry** symbol_table, const char* symbol, unsigned int address, bool entry_flag, bool external_flag) {
    // Allocate memory for the new entry
    SymbolTableEntry* new_entry = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
    if (!new_entry) {
        return NULL;
    }
    // Initialize the new entry
    strncpy(new_entry->symbol, symbol, MAX_SYMBOL_LENGTH);
    new_entry->address = address;
    new_entry->entry_flag = entry_flag;
    new_entry->external_flag = external_flag;
    new_entry->next = NULL;

    // If the symbol table is empty, make the new entry the head
    if (!*symbol_table) {
        *symbol_table = new_entry;
    }
    else {
        // Find the last entry in the symbol table
        SymbolTableEntry* current_entry = *symbol_table;
        while (current_entry->next) {
            current_entry = current_entry->next;
        }

        // Add the new entry to the end of the symbol table
        current_entry->next = new_entry;
    }

    return new_entry;
}
/* Function to free the symbol table */
void free_symbol_table(SymbolTable* table)
{
    SymbolNode* current = table->head;
    SymbolNode* next;

    while (current)
    {
        next = current->next;
        free(current);
        current = next;
    }

    table->head = NULL;
}
