#ifndef OUTPUT_GENERATOR_H
#define OUTPUT_GENERATOR_H

#include "symbol_table.h"
#include <stdbool.h>

bool generate_output_files(const char* input_filename);

/* Function to generate the object file */
void generate_object_file(const char* filename, const int* code_array, int code_array_size);

/* Function to generate the entry file */
void generate_entry_file(const char* filename, SymbolTable* symbol_table);

/* Function to generate the external file */
void generate_external_file(const char* filename, SymbolTable* symbol_table);

#endif /* OUTPUT_GENERATOR_H */