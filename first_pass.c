#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "first_pass.h"
#include "symbol_table.h"
#include "constants.h"
#include "instructions.h"
#include "opcode_table.h"
#include "utilities.h"
static int instruction_counter = 0;
static int data_counter = 0;
bool process_line_first_pass(const char* line);
bool first_pass(const char* base_input_filename) {
    char am_filename[MAX_FILENAME_LENGTH];
    char line[256];
    sprintf(am_filename, "%s.am", base_input_filename);
    FILE* file = fopen(am_filename, "r");
    if (!file) {
        perror("Error opening file");
        return false;
    }
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
bool is_line_contains_word(const char* line, const char* word) {
    char* token;
    char* delimiter = " ";
    char temp_line[256];
    /* Create a copy of the input line to prevent modification of the original line*/
    strncpy(temp_line, line, sizeof(temp_line));
    temp_line[sizeof(temp_line) - 1] = '\0';
    token = strtok(temp_line, delimiter);

    while (token != NULL) {
        if (strcmp(token, word) == 0) {
            return true;
        }
        token = strtok(NULL, delimiter);
    }
    return false;
}
bool process_line_first_pass(const char* line) {
    /* Variables for line processing*/
    char label[MAX_LABEL_LENGTH], opcode[MAX_OPCODE_LENGTH], operands[MAX_OPERANDS_LENGTH];
    bool has_label;
    /* Check for comments or empty lines and ignore them*/
    if (line[0] == ';' || line[0] == '\0') {
        return true;
    }
    /* Check for labels, add them to the symbol table if found*/
    has_label = parse_label(line, label);
    if (is_line_contains_word(line, STRING_DIRECTIVE) ||
        is_line_contains_word(line, DATA_DIRECTIVE))
    {
        if (has_label) {
            //add_to_symbol_table(label, instruction_counter);
        }
    }
    return true;
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