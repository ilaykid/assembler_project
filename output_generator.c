
#include <stdio.h>
#include "output_generator.h"
#include "symbol_table.h"

/* Function to generate the output file */
bool generate_output_files(const char* input_filename) {
    // Your implementation here

    // Return true if successful, false otherwise
    return true;
}

/* Function to generate the object file */
void generate_object_file(const char* filename, const int* code_array, int code_array_size) {
    FILE* file;
    int i;

    file = fopen(filename, "w");
    if (!file) {
        printf("Error opening object file: %s\n", filename);
        return;
    }

    for (i = 0; i < code_array_size; i++) {
        fprintf(file, "%04d %04X\n", i + 100, code_array[i]);
    }

    fclose(file);
}

/* Function to generate the entry file */
void generate_entry_file(const char* filename, SymbolTable* symbol_table) {
    FILE* file;
    SymbolTable* entry;

    file = fopen(filename, "w");
    if (!file) {
        printf("Error opening entry file: %s\n", filename);
        return;
    }

    entry = symbol_table->head;
    while (entry) {
        if (entry->entry_flag) {
            fprintf(file, "%s %04d\n", entry->symbol, entry->address);
        }
        entry = entry->next;
    }

    fclose(file);
}

/* Function to generate the external file */
void generate_external_file(const char* filename, SymbolTable* symbol_table) {
    FILE* file;
    SymbolTable* entry;

    file = fopen(filename, "w");
    if (!file) {
        printf("Error opening external file: %s\n", filename);
        return;
    }

    entry = symbol_table->head;
    while (entry) {
        if (entry->external_flag) {
            fprintf(file, "%s %04d\n", entry->symbol, entry->address);
        }
        entry = entry->next;
    }

    fclose(file);
}