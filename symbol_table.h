#include "constants.h"
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
typedef struct SymbolTableEntry {
    char symbol[MAX_SYMBOL_LENGTH + 1];
    unsigned int address;
    bool relocatable;
    bool data_part;
    struct SymbolTableEntry* next;
} SymbolTableEntry;

bool add_to_symbol_table(const char* symbol, unsigned int address,
    bool relocatable, bool data_part);
//
//#define MAX_LABEL_LENGTH 32
//
//typedef enum
//{
//    SYMBOL_ATTRIBUTE_NONE,
//    SYMBOL_ATTRIBUTE_ENTRY,
//    SYMBOL_ATTRIBUTE_EXTERN
//} SymbolAttribute;
//
//typedef struct SymbolNode
//{
//    char label[MAX_LABEL_LENGTH];
//    int address;
//    SymbolAttribute attribute;
//    struct SymbolNode* next;
//} SymbolNode;
//typedef struct SymbolTableEntry {
//    char* symbol;
//    int address;
//} SymbolTable;
///* Function to create a new symbol node */
//SymbolNode* new_symbol_node(const char* label, int address, SymbolAttribute attribute);
//
////void add_symbol(SymbolTable* table, const char* label, int address, SymbolAttribute attribute);
//
///* Function to search for a symbol in the symbol table */
//SymbolNode* find_symbol(SymbolTable* table, const char* label);
//
///* Function to free the symbol table */
//void free_symbol_table(SymbolTable* table);
//
//SymbolTable* init_symbol_table();
///* Function to add a symbol to the symbol table */
//bool add_to_symbol_table(const char* symbol, unsigned int address);
#endif /* SYMBOL_TABLE_H */
