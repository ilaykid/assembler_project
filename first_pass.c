#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "first_pass.h"
#include "symbol_table.h"
#include "constants.h"
#include "instructions.h"
#include "opcode_table.h"

bool process_line_first_pass(const char* line);
bool first_pass(const char* filename) {
    FILE* file = fopen(filename, "r");

    if (!file) {
        perror("Error opening file");
        return false;
    }

    // Initialize symbol table
    init_symbol_table();

    char line[256];
    int line_number = 1;
    bool success = true;

    // Read and process lines in the assembly file
    while (fgets(line, sizeof(line), file)) {
        // Process the line
        if (!process_line_first_pass(line)) {
            fprintf(stderr, "Error: Syntax error at line %d\n", line_number);
            success = false;
        }
        line_number++;
    }

    fclose(file);
    return success;
}
/* Process an instruction from the given line */
bool process_instruction(const char* instruction, int line_number, int* instruction_counter) {
    char opcode[MAX_OPCODE_LENGTH + 1];
    char operands[MAX_OPERANDS_LENGTH + 1];
    Instruction instr;

    // Parse the instruction to get the opcode and operands.
    if (parse_instruction(instruction, opcode, operands)) {
        // Look up the opcode in the opcode table.
        OpcodeTableEntry* entry = opcode_table_lookup(opcode);
        if (entry != NULL) {
            // Set the opcode and operands in the instruction structure.
            instr.opcode = entry->opcode;
            strncpy(instr.operands, operands, MAX_OPERANDS_LENGTH);
            instr.operands[MAX_OPERANDS_LENGTH] = '\0';

            // Update the instruction counter based on the number of words the instruction takes.
            *instruction_counter += entry->num_words;
        }
        else {
            printf("Error: Unknown opcode '%s' at line %d\n", opcode, line_number);
            return false;
        }
    }
    else {
        printf("Error: Failed to parse instruction at line %d\n", line_number);
        return false;
    }
    return true;
}
bool process_line_first_pass(const char* line) {
    // Variables for line processing
    char label[MAX_LABEL_LENGTH], opcode[MAX_OPCODE_LENGTH], operands[MAX_OPERANDS_LENGTH];
    bool has_label;
    int instruction_counter = 0;
    // Remove leading and trailing whitespace from the line
    // ...

    // Check for comments or empty lines and ignore them
    if (line[0] == ';' || line[0] == '\0') {
        return true;
    }

    // Check for labels, add them to the symbol table if found
    has_label = parse_label(line, label);
    if (has_label) {
        add_to_symbol_table(label, instruction_counter);
    }

    // Parse the instruction, identify opcode and operands
    if (!parse_instruction(line, opcode, operands, has_label)) {
        printf("Error: Failed to parse instruction on line: %s\n", line);
        return false;
    }

    // Process the instruction and update the instruction counter
    if (!process_instruction(opcode, operands, &instruction_counter)) {
        printf("Error: Failed to process instruction on line: %s\n", line);
        return false;
    }

    // Return true if the line processing is successful
    return true;
}