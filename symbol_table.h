#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_LABEL_LENGTH 32

typedef enum
{
    SYMBOL_ATTRIBUTE_NONE,
    SYMBOL_ATTRIBUTE_ENTRY,
    SYMBOL_ATTRIBUTE_EXTERN
} SymbolAttribute;

typedef struct SymbolNode
{
    char label[MAX_LABEL_LENGTH];
    int address;
    SymbolAttribute attribute;
    struct SymbolNode* next;
} SymbolNode;

typedef struct
{
    SymbolNode* head;
} SymbolTable;

typedef struct SymbolTableEntry {
    char* symbol;
    int address;
    int entry_flag;
    int external_flag;
    struct SymbolTableEntry* next;
} SymbolTableEntry;
/* Function to create a new symbol node */
SymbolNode* new_symbol_node(const char* label, int address, SymbolAttribute attribute);

/* Function to add a symbol to the symbol table */
void add_symbol(SymbolTable* table, const char* label, int address, SymbolAttribute attribute);

/* Function to search for a symbol in the symbol table */
SymbolNode* find_symbol(SymbolTable* table, const char* label);

/* Function to free the symbol table */
void free_symbol_table(SymbolTable* table);

SymbolTableEntry* init_symbol_table();

#endif /* SYMBOL_TABLE_H */
