#pragma once
#include "opcode_table.h"
#include "constants.h"
#include <stdbool.h>
#include "symbol_table.h"
#ifndef UTILITIES_H
#define UTILITIES_H
typedef struct {
    unsigned char* code_line[MAX_CODE_ARRAY_SIZE];
} Insturctions;
typedef struct {
    SymbolTableEntry* symbol_table;
    unsigned int instruction_counter;
    unsigned int data_counter;
    unsigned int entry_counter;
    unsigned int extern_counter;
    unsigned int code_length;
    unsigned int data_length;
    Insturctions code_image;
    Insturctions data_image;
} AssemblerState;

extern AssemblerState global_state;
extern OpcodeTableEntry opcode_table[];

bool is_line_contains_word(const char* line, const char* word);
char* encode_unique_base_2(int num, int count_bits);
void init_global_state();
/* Function to remove whitespace from the beginning and end of a string */
void trim_whitespace(char* str);

void fill_string_with_dots(char str[], int length);

/* Function to split a string using the specified delimiter */
char** split_string(const char* str, const char* delimiter, int* count);

/* Function to free the memory allocated by split_string */
void free_split_string(char** str_array, int count);

/* Function to parse a label from a line of assembly code */
bool parse_label(const char* line, char* label);
/* Parses an instruction from a given line */
bool parse_instruction(const char* line, char* instruction,int skip_chars);

const OpcodeTableEntry* opcode_table_lookup(const char* opcode);

#endif /* UTILITIES_H */