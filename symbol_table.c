#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "symbol_table.h"
#include "constants.h"

/* Singleton instance of the symbol table */
static SymbolTable* symbol_table = NULL;

/* Get the singleton instance of the symbol table*/
SymbolTable* get_symbol_table_instance() {
    if (symbol_table == NULL) {
        symbol_table = init_symbol_table();
    }
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

///* Function to add a symbol to the symbol table */
//void add_symbol(SymbolTable* table, const char* label, int address, SymbolAttribute attribute)
//{
//    SymbolNode* new_node = new_symbol_node(label, address, attribute);
//
//    if (table->head == NULL)
//    {
//        table->head = new_node;
//    }
//    else
//    {
//        SymbolNode* current = table->head;
//        while (current->next)
//        {
//            current = current->next;
//        }
//        current->next = new_node;
//    }
//}

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
bool add_to_symbol_table(const char* symbol, unsigned int address) {
    /* Allocate memory for the new entry*/
    SymbolTable* table = get_symbol_table_instance();
    if (!table) {
        return NULL;
    }
    /* Initialize the new entry*/
    strncpy(table->symbol, symbol, MAX_SYMBOL_LENGTH);
    table->address = address;
    
    //add the symo
    return table;
}