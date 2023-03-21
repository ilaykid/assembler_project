#include "opcode_table.h"
#include <stdbool.h>
#ifndef UTILITIES_H
#define UTILITIES_H
extern OpcodeTableEntry opcode_table[];

/* Function to remove whitespace from the beginning and end of a string */
void trim_whitespace(char* str);

/* Function to split a string using the specified delimiter */
char** split_string(const char* str, const char* delimiter, int* count);

/* Function to free the memory allocated by split_string */
void free_split_string(char** str_array, int count);

/* Function to parse a label from a line of assembly code */
bool parse_label(const char* line, char* label);
/* Parses an instruction from a given line */
bool parse_instruction(const char* line, char* instruction);

const OpcodeTableEntry* opcode_table_lookup(const char* opcode);

#endif /* UTILITIES_H */