#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "first_pass.h"
#include "symbol_table.h"
#include "constants.h"
#include "instructions.h"
#include "opcode_table.h"
#include "utilities.h"
#include "operand.h"
AssemblerState global_state;
bool handle_operands(char* line, const char* directive);
bool parse_directive(const char* line, char* directive);
bool parse_opcode(const char* line, char* opcode);
bool is_addressing_mode_relative(const char* instruction);
bool process_line_first_pass(const char* line);
bool is_line_contains_opcode(const char* line);
bool handle_instruction_pass_one(char* line, int line_number,int skip_chars);
const OpcodeTableEntry* get_opcode(const char* line, int skip_chars) {
	char instruction[MAX_LABEL_LENGTH];
	if (!parse_instruction(line, instruction, skip_chars)) {
		// No instruction found
		return NULL;
	}

	const OpcodeTableEntry* opcode_entry = opcode_table_lookup(instruction);
	if (opcode_entry == NULL) {
		// Opcode not found in the table
		printf("Error: invalid opcode '%s'\n", instruction);
		return NULL;
	}

	return opcode_entry;
}
bool first_pass(const char* base_input_filename) {
	init_global_state(global_state);
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
		/* Remove trailing newline character*/
		strtok(line, "\n");
		if (!process_line_first_pass(line, line_number)) {
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
			instr.mnemonic = entry->mnemonic;
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
	const char* result = strstr(line, word);
	return (result != NULL);
}

bool process_line_first_pass(const char* line, int line_number) {
	/* Variables for line processing*/
	char label[MAX_LABEL_LENGTH],
		opcode[MAX_OPCODE_LENGTH],
		operands[MAX_OPERANDS_LENGTH];
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
			if (!add_to_symbol_table(label, global_state.data_counter, true, true, DATA_DIRECTIVE)) {
				printf("Error: Symbol %s already defined\n", label);
				return false;
			}
		}
		// Update data counter according to data or string length
		int data_length = process_data_or_string(line);
		global_state.data_counter += data_length;
		return true;
	}
	/*Check for extern directive*/
	if (is_line_contains_word(line, EXTERN_DIRECTIVE)) {
		/* Insert symbol into symbol table with external flag and no value*/
		//char* operand = get_operand(line);
		if (!handle_operands(line, EXTERN_DIRECTIVE, false)) {
			printf("Error: Symbol %s already defined\n", ENTRY_DIRECTIVE);
			return false;
		}
		return true;
	}
	// Check for entry directive
	if (is_line_contains_word(line, ENTRY_DIRECTIVE)) {
		// Insert symbol into symbol table with entry flag and no value

		if (!handle_operands(line, ENTRY_DIRECTIVE, true)) {
			printf("Error: Symbol %s already defined\n", ENTRY_DIRECTIVE);
			return false;
		}
		return true;
	}
	if (has_label)
	{
		if (!add_to_symbol_table(label, global_state.instruction_counter
			, true, true,"code")) {
			printf("Error: Symbol %s already defined\n", label);
			return false;
		}
	}
	int skip_chars = strlen(label)+1;
	if (!handle_instruction_pass_one(line,line_number,skip_chars))
		return false;
	//if (is_line_contains_opcode(line)) {
	//	int operand_length = process_instruction(line, line_number, global_state.instruction_counter);
	//	int instruction_length = 1; // start with opcode length of 1 word

	//	if (operand_length > 0) { // check if instruction has operands
	//		instruction_length++; // add 1 word for the first operand
	//		if (operand_length > 1) { // check if instruction has a second operand
	//			instruction_length++; // add 1 word for the second operand
	//			if (is_addressing_mode_relative(line)) { // check if the instruction has relative addressing mode
	//				instruction_length++; // add 1 word for the relative addressing mode
	//			}
	//		}
	//	}
	//	// update instruction counter with instruction length
	//	global_state.instruction_counter += instruction_length;
	//}
	// Parse the instruction, identify opcode and operands
	//if (!parse_instruction(line, opcode, operands, has_label)) {
	//    printf("Error: Failed to parse instruction on line: %s\n", line);
	//    return false;
	//}

	//// Process the instruction and update the instruction counter
	//if (!process_instruction(opcode, operands, &instruction_counter)) {
	//    printf("Error: Failed to process instruction on line: %s\n", line);
	//    return false;
	//}

	// Return true if the line processing is successful
	return true;
}
bool handle_instruction_pass_one(char* line, int line_number,int skip_chars) {
	char opcode[MAX_OPCODE_LENGTH];
	if (get_opcode(line,skip_chars)) { // get the opcode from the instruction
			int operand_length = process_instruction(line, line_number, global_state.instruction_counter);
			int instruction_length = 1; // start with opcode length of 1 word

			if (operand_length > 0) { // check if instruction has operands
				instruction_length++; // add 1 word for the first operand
				if (operand_length > 1) { // check if instruction has a second operand
					instruction_length++; // add 1 word for the second operand
					if (is_addressing_mode_relative(line)) { // check if the instruction has relative addressing mode
						instruction_length++; // add 1 word for the relative addressing mode
					}
				}
			

			// update instruction counter with instruction length
			global_state.instruction_counter += instruction_length;
			return true; // instruction processed successfully
		}
		else {
			// opcode is not valid, report error
			printf("Error at line %d: Invalid opcode '%s'\n", line_number, opcode);
			return false;
		}
	}
	// line does not contain opcode, skip
	return true;
}
bool handle_operands(char* line, const char* directive, bool is_relo) {
	// Find the directive in the line
	char* ptr = line + strlen(directive);
	char operand[MAX_OPERANDS_LENGTH];
	int counter;
	while (ptr != line + strlen(line))
	{
		counter = 0;
		while (*ptr == ' ' || *ptr == '\t') {
			ptr++;
		}
		while (isalpha(*ptr) || isdigit(*ptr))
		{
			operand[counter++] = *ptr;
			ptr++;
		}
		operand[counter] = '\0';
		add_to_symbol_table(operand, 0, is_relo, false, directive);

	}
	// Skip the directive and any whitespace


	// Update the line pointer to point to the beginning of the operands
	//line = ptr;
}
/* Check if the given line contains an opcode */
bool is_line_contains_opcode(const char* line) {
	char opcode[MAX_OPCODE_LENGTH + 1];

	// Attempt to parse the opcode from the line
	if (parse_opcode(line, opcode)) {
		// Look up the opcode in the opcode table
		return (opcode_table_lookup(opcode) != NULL);
	}
	else {
		return false;
	}
}
/* Process a data or string directive from the given line and return the number of words it occupies */
int process_data_or_string(const char* line) {
	char directive[MAX_LABEL_LENGTH + 1];
	char* ptr = line;
	int chars_before_directive_counter = 0;
	// Parse the directive from the line
	if (!parse_directive(line, directive, &chars_before_directive_counter)) {
		printf("Error: Invalid directive\n");
		return 0;
	}

	// Move the pointer after the directive name
	ptr += chars_before_directive_counter;
	// Check if this is a .data directive
	if (strcmp(directive, DATA_DIRECTIVE) == 0) {
		char* endptr;
		int data_word_count = 0;

		while (*ptr) {
			// Convert the next number to int
			int num = strtol(ptr, &endptr, 10);
			if (ptr == endptr) {
				// No number was found
				printf("Error: Invalid number in .data directive\n");
				return 0;
			}

			// Add the number to the data image
			global_state.data_image[global_state.data_counter++] = num;
			data_word_count++;

			ptr = endptr;

			// Skip white spaces and commas
			while (isspace(*ptr) || *ptr == ',') {
				ptr++;
			}
		}

		return data_word_count;
	}
	while (isspace(*ptr)) {
		ptr++;
	}
	// Check if this is a .string directive
	if (strcmp(directive, STRING_DIRECTIVE) == 0) {
		if (*ptr != '\"') {
			printf("Error: Invalid .string directive\n");
			return 0;
		}

		// Skip the opening quotation mark
		ptr++;

		int str_length = 0;

		while (*ptr && *ptr != '\"') {
			if (str_length >= MAX_STRING_LENGTH) {
				printf("Error: String is too long\n");
				return 0;
			}

			global_state.data_image[global_state.data_counter++] = *ptr;
			ptr++;
			str_length++;
		}

		if (*ptr == '\0') {
			printf("Error: Invalid .string directive\n");
			return 0;
		}

		/*Add the terminating null character to the string*/
		global_state.data_image[global_state.data_counter++] = '\0';
		str_length++;
		return str_length;
	}

	printf("Error: Invalid directive\n");
	return 0;
}
bool is_addressing_mode_relative(const char* instruction) {
	char opcode[MAX_OPCODE_LENGTH + 1];
	char operands[MAX_OPERANDS_LENGTH + 1];

	// Parse the instruction to get the opcode and operands
	if (!parse_instruction(instruction, opcode, operands)) {
		return false;
	}

	// Look up the opcode in the opcode table
	OpcodeTableEntry* entry = opcode_table_lookup(opcode);
	if (entry == NULL) {
		return false;
	}

	// Check if the opcode uses relative addressing mode
	//return entry-> addressing_modes[0] == RELATIVE;
	return false; //temmp
}
/* Parse the opcode from the given line */
bool parse_opcode(const char* line, char* opcode) {
	// Copy the line to a temporary buffer to tokenize it
	char buffer[MAX_LINE_LENGTH + 1];
	strcpy(buffer, line);

	// Tokenize the line and extract the first token as the opcode
	char* token = strtok(buffer, " \t");
	if (token != NULL) {
		strncpy(opcode, token, MAX_OPCODE_LENGTH);
		opcode[MAX_OPCODE_LENGTH] = '\0';
		return true;
	}

	return false;
}
/* Parse the directive from the given line */
bool parse_directive(const char* line, char* directive, int* chars_before_directive_counter) {
	int i = 0;
	int j = 0;
	// Check for a dot at the beginning of the line
	while (line[i] != '.' && i < strlen(line))
		i++;
	if (i == strlen(line))
		return false;
	directive[j] = '.';
	j++; i++;
	// Copy the directive into the output string
	while (isalpha(line[i]) && line[i] != ' ' && i < MAX_LABEL_LENGTH) {
		directive[j] = line[i];
		i++; j++;
	}

	directive[j] = '\0';
	*chars_before_directive_counter = i;

}