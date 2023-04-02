#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "first_pass.h"
#include "symbol_table.h"
#include "constants.h"
#include "opcode_table.h"
#include "utilities.h"
#include "operand.h"
bool handle_operands(char* line, char* directive, bool is_relo);
char* build_first_word_binary(OpcodeTableEntry* opcode_entry, int operands_num, Operand  operands[3], int line_number);
bool parse_directive(char* line, char* directive, int* chars_before_directive_counter);
bool parse_opcode(char* line, char* opcode);
bool process_line_first_pass(char* line, int line_number);
bool handle_extern(char* line);
int process_data_or_string(char* line);
bool handle_data_or_string(bool has_label, char  label[16], char* line);
bool is_line_contains_opcode(char* line);
void calculate_instruction_length_and_update_IC(char* line, int line_number,
	int skip_chars);
bool first_pass(char* base_input_filename) {
	char am_filename[MAX_FILENAME_LENGTH];
	char line[256];
	FILE* file;
	int line_number;
	bool success;
	init_global_state();
	sprintf(am_filename, "%s.am", base_input_filename);
	file = fopen(am_filename, "r");
	if (!file) {
		perror("Error opening file");
		return false;
	}
	line_number = 1;
	success = true;
	/*/ Read and process lines in the assembly file*/
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
bool process_line_first_pass(char* line, int line_number) {
	/* Variables for line processing*/
	char label[MAX_LABEL_LENGTH];
	bool has_label;
	int skip_chars = 0;
	/* Check for comments or empty lines and ignore them*/
	if (line[0] == ';' || line[0] == '\0') {
		return true;
	}
	/* Check for labels, add them to the symbol table if found*/
	has_label = parse_label(line, label);
	if (is_line_contains_word(line, STRING_DIRECTIVE) ||
		is_line_contains_word(line, DATA_DIRECTIVE))
	{
		return handle_data_or_string(has_label, label, line);
	}
	/*Check for extern directive*/
	if (is_line_contains_word(line, EXTERN_DIRECTIVE)) {
		return handle_extern(line);
	}
	/*/ Check for entry directive*/
	if (is_line_contains_word(line, ENTRY_DIRECTIVE)) /*/handle it on second pass*/
		return true;
	if (has_label)
	{
		skip_chars = strlen(label) + 2; /* 2 for ": "*/

		if (!add_to_symbol_table(label, global_state.instruction_counter
			, true, true, "code")) {
			printf("Error: Symbol %s already defined\n", label);
			return false;
		}
	}
	calculate_instruction_length_and_update_IC(line, line_number, skip_chars);
	add_ic_to_all_data(global_state.instruction_counter);
	return true;
}
bool handle_extern(char* line)
{
	/* Insert symbol into symbol table with external flag and no value*/
	if (!handle_operands(line, EXTERN_DIRECTIVE, false)) {
		printf("Error: Symbol %s already defined\n", ENTRY_DIRECTIVE);
		return false;
	}
	global_state.extern_counter++;
	return true;
}
bool handle_data_or_string(bool has_label, char  label[16], char* line)
{
	int data_length;
	if (has_label) {
		if (!add_to_symbol_table(label, global_state.data_counter,
			true, true, DATA_DIRECTIVE)) {
			printf("Error: Symbol %s already defined\n", label);
			return false;
		}
	}
	/*/ Update data counter according to data or string length*/
	data_length = process_data_or_string(line);
	global_state.data_counter += data_length;
	return true;
}
/* Calculate the length of the instruction and update the instruction counter */
void calculate_instruction_length_and_update_IC(char* line, int line_number,
	int skip_chars) {
	OpcodeTableEntry* opcode_entry = get_opcode(line, skip_chars);
	Operand operands[MAX_OPERANDS];
	int operands_num;
	int instruction_length;
	char* first_word_binary;
	line = line + skip_chars;
	operands_num = handle_and_count_operands(line, line_number
		, global_state.instruction_counter, opcode_entry->mnemonic, operands);
	instruction_length = 1 + operands_num;
	/* update instruction counter with instruction length*/
	first_word_binary = build_first_word_binary(opcode_entry, operands_num, operands, line_number);
	global_state.code_image.code_line[(line_number - 1) * 4] = first_word_binary;/*first code row for that row*/
	global_state.instruction_counter += instruction_length;
}
char* build_first_word_binary(OpcodeTableEntry* opcode_entry, int operands_num, Operand  operands[3], int line_number)
{
	char era_method[ERA_BITS + 1];
	char address_method_dest[OPERAND_ADDR_BITS + 1];
	char address_method_source[OPERAND_ADDR_BITS + 1];
	char opcode_binary[OPCODE_BITS + 1];
	char param1[PARAM_BITS + 1];
	char param2[PARAM_BITS + 1];
	char* instruction_binary = malloc((WORD_SIZE + 1) * sizeof(char));
	strcpy(instruction_binary, "");
	strcpy(era_method, ABSOLUTE_ENCODING);
	strcpy(opcode_binary, encode_unique_base_2(opcode_entry->opcode_index, OPCODE_BITS));
	fill_string_with_dots(param1, PARAM_BITS);
	fill_string_with_dots(param2, PARAM_BITS);
	switch (operands_num)
	{
	case 0: /* rts || stop */
	{
		fill_string_with_dots(address_method_dest, OPERAND_ADDR_BITS);
		fill_string_with_dots(address_method_source, OPERAND_ADDR_BITS);
		break;
	}
	case 1: /* jump with only label || simple opcodes*/
	{
		fill_string_with_dots(address_method_source, OPERAND_ADDR_BITS);
		strcpy(address_method_dest, encode_unique_base_2(operands[0].value, OPERAND_ADDR_BITS));
		if (operands[0].addressing_method == DIRECT)
		{
			strcpy(era_method, RELOCATABLE_ENCODING);
		}
		else
		{
			strcpy(era_method, ABSOLUTE_ENCODING);
		}
		break;
	}
	case 2:
	{
		strcpy(address_method_dest, encode_unique_base_2(operands[0].value, OPERAND_ADDR_BITS));
		strcpy(address_method_source, encode_unique_base_2(operands[1].value, OPERAND_ADDR_BITS));
		if (operands[0].addressing_method == DIRECT ||
			operands[1].addressing_method == DIRECT)
			strcpy(era_method, RELOCATABLE_ENCODING);
		else
			strcpy(era_method, ABSOLUTE_ENCODING);
		break;
	}
	case 3:/*jump with label and parameters*/
	{
		fill_string_with_dots(address_method_dest, OPERAND_ADDR_BITS);
		fill_string_with_dots(address_method_source, OPERAND_ADDR_BITS);
		strcpy(era_method, EXTERNAL_ENCODING);
		strcpy(param1, encode_unique_base_2(operands[1].addressing_method, PARAM_BITS));
		strcpy(param2, encode_unique_base_2(operands[2].addressing_method, PARAM_BITS));
		break;
	}
	default:
		printf("Error: problems with operands in line %d\n", line_number);
		break;
	}
	strcpy(address_method_dest, encode_unique_base_2(operands[0].
		addressing_method, OPERAND_ADDR_BITS));
	param1[PARAM_BITS] = '\0';
	param2[PARAM_BITS] = '\0';
	opcode_binary[OPCODE_BITS] = '\0';
	address_method_source[OPERAND_ADDR_BITS] = '\0';
	address_method_dest[OPERAND_ADDR_BITS] = '\0';
	era_method[ERA_BITS] = '\0';
	strcat(instruction_binary, param1);
	strcat(instruction_binary, param2);
	strcat(instruction_binary, opcode_binary);
	strcat(instruction_binary, address_method_source);
	strcat(instruction_binary, address_method_dest);
	strcat(instruction_binary, era_method);
	instruction_binary[WORD_SIZE] = '\0';
	return instruction_binary;
}
bool handle_operands(char* line, char* directive, bool is_relo) {
	/* Find the directive in the line */
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
	return true;
}
/* Check if the given line contains an opcode */
bool is_line_contains_opcode(char* line) {
	char opcode[MAX_OPCODE_LENGTH + 1];

	/* Attempt to parse the opcode from the line*/
	if (parse_opcode(line, opcode)) {
		/* Look up the opcode in the opcode table*/
		return (opcode_table_lookup(opcode) != NULL);
	}
	else {
		return false;
	}
	return true;
}
/* Process a data or string directive from the given line and return the number of words it occupies */
int process_data_or_string(char* line) {
	char directive[MAX_LABEL_LENGTH + 1];
	char* ptr = line;
	int chars_before_directive_counter = 0;
	/* Parse the directive from the line*/
	if (!parse_directive(line, directive, &chars_before_directive_counter)) {
		printf("Error: Invalid directive\n");
		return 0;
	}

	/* Move the pointer after the directive name*/
	ptr += chars_before_directive_counter;
	/*/ Check if this is a .data directive*/
	if (strcmp(directive, DATA_DIRECTIVE) == 0) {
		char* endptr;
		int data_word_count = 0;

		while (*ptr) {
			/*/ Convert the next number to int*/
			int num = strtol(ptr, &endptr, 10);
			if (ptr == endptr) {
				/*/ No number was found*/
				printf("Error: Invalid number in .data directive\n");
				return 0;
			}

			/*/ Add the number to the data image*/
			global_state.data_image.code_line[global_state.data_counter++]
				= encode_unique_base_2(num, WORD_SIZE);
			data_word_count++;

			ptr = endptr;

			/* Skip white spaces and commas*/
			while (isspace(*ptr) || *ptr == ',') {
				ptr++;
			}
		}
		return data_word_count;
	}
	while (isspace(*ptr)) {
		ptr++;
	}
	/** Check if this is a.string directive*/
	if (strcmp(directive, STRING_DIRECTIVE) == 0) {
		int str_length = 0;
		if (*ptr != '\"') {
			printf("Error: Invalid .string directive\n");
			return 0;
		}

		/*/ Skip the opening quotation mark*/
		ptr++;
		while (*ptr && *ptr != '\"') {
			if (str_length >= MAX_STRING_LENGTH) {
				printf("Error: String is too long\n");
				return 0;
			}

			global_state.data_image.code_line[global_state.data_counter++] =
				encode_unique_base_2(*ptr, WORD_SIZE);
			ptr++;
			str_length++;
		}

		if (*ptr == '\0') {
			printf("Error: Invalid .string directive\n");
			return 0;
		}

		/*Add the terminating null character to the string*/
		global_state.data_image.code_line[global_state.data_counter++] = '\0';
		str_length++;
		return str_length;
	}

	printf("Error: Invalid directive\n");
	return 0;
}
/* Parse the opcode from the given line */
bool parse_opcode(char* line, char* opcode) {
	/*/ Copy the line to a temporary buffer to tokenize it*/
	char buffer[MAX_LINE_LENGTH + 1];
	char* token;
	strcpy(buffer, line);
	/*/ Tokenize the line and extract the first token as the opcode*/
	token = strtok(buffer, " \t");
	if (token != NULL) {
		strncpy(opcode, token, MAX_OPCODE_LENGTH);
		opcode[MAX_OPCODE_LENGTH] = '\0';
		return true;
	}

	return false;
}
/* Parse the directive from the given line */
bool parse_directive(char* line, char* directive, int* chars_before_directive_counter) {
	int i = 0;
	int j = 0;
	/*/ Check for a dot at the beginning of the line*/
	while (line[i] != '.' && i < strlen(line))
		i++;
	if (i == strlen(line))
		return false;
	directive[j] = '.';
	j++; i++;
	/*/ Copy the directive into the output string*/
	while (isalpha(line[i]) && line[i] != ' ' && i < MAX_LABEL_LENGTH) {
		directive[j] = line[i];
		i++; j++;
	}

	directive[j] = '\0';
	*chars_before_directive_counter = i;
	return true;
}