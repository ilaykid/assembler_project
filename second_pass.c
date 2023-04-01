#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "utilities.h"
#include "operand.h"
bool insert_entry(char* line);
void handle_two_direct_registers(char  instruction_binary[15], char  address[13], Operand  operands[3], char  era_method[3]);
void handle_jump_line(char  instruction_binary[15], char  address[13], Operand  operands[3], char  era_method[3], int line_num);
int second_pass(const char* base_input_filename) {
	// Initialize variables
	int error_count = 0;
	int IC = 0; // Initialize IC to 0
	int line_num = 0;
	char line[MAX_LINE_LENGTH];
	char* tokens[MAX_LINE_WORDS_COUNT];
	char am_filename[MAX_FILENAME_LENGTH];

	global_state.instruction_counter = 0;
	sprintf(am_filename, "%s.am", base_input_filename);
	// Open input file
	FILE* input_file = fopen(am_filename, "r");
	if (input_file == NULL) {
		printf("Error: Failed to open input file: %s\n", am_filename);
		return 0;
	}
	// Read input file line by line
	while (fgets(line, MAX_LINE_LENGTH, input_file)) {
		line_num++;
		trim_whitespace(line);
		char* orig_line="";
		strcpy(orig_line, line);
		// Split line into tokens
		int num_tokens = split_line(line, tokens);
		int skip_chars = 0;
		// Skip empty lines and comments
		if (num_tokens == 0 || tokens[0][0] == ';') {
			continue;
		}
		if (tokens[0][strlen(tokens[0]) - 1] == ':') {
			tokens[0][strlen(tokens[0]) - 1] = '\0';
		}
		// Check if line contains a symbol

		if (is_valid_label(tokens[0])) {
			skip_chars = strlen(tokens[0]) + 2;
			// Skip symbol and process instruction/directive
			num_tokens--;
			memmove(tokens, tokens + 1, num_tokens * sizeof(char*));
		}
		/* Check if line is a directive that we skip in the second pass*/
		if (is_line_contains_word(orig_line, STRING_DIRECTIVE) ||
			is_line_contains_word(orig_line, DATA_DIRECTIVE) ||
			is_line_contains_word(orig_line, EXTERN_DIRECTIVE))
			continue;
		else if (is_line_contains_word(orig_line, ENTRY_DIRECTIVE)) {
			/* Insert symbol into symbol table with entry flag and no value*/
			if (!insert_entry(tokens[1])) {
				printf("Error: Symbol %s already defined\n", ENTRY_DIRECTIVE);
			}
			continue;
		}

		OpcodeTableEntry* opcode_entry = get_opcode(orig_line, skip_chars);
		Operand operands[MAX_OPERANDS];
		orig_line= orig_line + skip_chars;
		int operands_num = handle_and_count_operands(orig_line, line_num
			, global_state.instruction_counter, opcode_entry->mnemonic, operands);
		int instruction_length = 1 + operands_num;
		char era_method[ERA_BITS + 1];
		char address[ADDRESS_BITS + 1];
		char instruction_binary[WORD_SIZE + 1];
		instruction_binary[0] = 0;
		if (is_jump_opcode(opcode_entry->mnemonic))
		{
			/*first operand - label*/
			instruction_binary[0] = 0;
			strcpy(address, encode_unique_base_2(operands[0].value, ADDRESS_BITS));
			strcpy(era_method, EXTERNAL_ENCODING);
			strcat(instruction_binary, address);
			strcat(instruction_binary, era_method);
			instruction_binary[WORD_SIZE] = '\0';
			global_state.code_image.code_line[((line_num - 1) * 4) + 1] = instruction_binary;
			if (operands_num > 2)
			{
				if (operands[1].addressing_method == REGISTER_DIRECT &&/*merge two registers into one line*/
					operands[2].addressing_method == REGISTER_DIRECT)
				{
					handle_two_direct_registers(instruction_binary, address, operands,
						era_method,line_num,1);
				}
				else
				{
					/*first reg*/
					handle_jump_line(instruction_binary, address, operands, era_method, line_num,1);
					/*second reg*/
					handle_jump_line(instruction_binary, address, operands, era_method, line_num,2);
				}
			}
		}
		else if (operands[0].addressing_method == REGISTER_DIRECT && /*merge two registers into one line*/
			operands[1].addressing_method == REGISTER_DIRECT)
		{

			handle_two_direct_registers(instruction_binary, address, operands,
				era_method, line_num, 0);

		}
		else for (int i = 0; i < operands_num; i++)
		{
			instruction_binary[0] = 0;
			Operand this_op = operands[i];
			if (this_op.addressing_method == IMMEDIATE)
			{
				strcpy(address, encode_unique_base_2(this_op.value, ADDRESS_BITS));
				strcpy(era_method, ABSOLUTE_ENCODING);
				strcat(instruction_binary, address);
				strcat(instruction_binary, era_method);
			}
			else if (this_op.addressing_method == DIRECT)
			{
				strcpy(era_method, RELOCATABLE_ENCODING);
				strcpy(address, encode_unique_base_2(this_op.value, ADDRESS_BITS));
				strcat(instruction_binary, address);
				strcat(instruction_binary, era_method);
			}
			else if (this_op.addressing_method == REGISTER_DIRECT)
			{
				strcpy(era_method, RELOCATABLE_ENCODING);
				strcpy(address, encode_unique_base_2(this_op.value, ADDRESS_BITS));
				strcat(instruction_binary, address);
				strcat(instruction_binary, era_method);
			}
			instruction_binary[WORD_SIZE] = '\0';
			global_state.code_image.code_line[((line_num - 1) * 4) + i + 1] = instruction_binary;
		}
		global_state.instruction_counter += instruction_length;
	}
	// Close input and output files
	fclose(input_file);
	return error_count == 0; // Return success if there were no errors
}
void handle_two_direct_registers(char  instruction_binary[WORD_SIZE + 1], char  address[ADDRESS_BITS + 1],
	Operand  operands[MAX_OPERANDS], char  era_method[ERA_BITS + 1],int line_num,
	int index)
{
	instruction_binary[0] = 0;
	strcpy(address, encode_unique_base_2(operands[index].value, ADDRESS_BITS / 2));
	strcat(address, encode_unique_base_2(operands[index+1].value, ADDRESS_BITS / 2));
	strcpy(era_method, ABSOLUTE_ENCODING);
	strcat(instruction_binary, address);
	strcat(instruction_binary, era_method);
	instruction_binary[WORD_SIZE] = '\0';
	global_state.code_image.code_line[((line_num - 1) * 4)+1+index] = instruction_binary;

}
void handle_jump_line(char instruction_binary[WORD_SIZE+1], char  address[ADDRESS_BITS+1]
	, Operand  operands[MAX_OPERANDS], char  era_method[ERA_BITS+1], int line_num,int idx_operand)
{
	instruction_binary[0] = 0;
	strcpy(address, encode_unique_base_2(operands[idx_operand].value, ADDRESS_BITS));
	strcpy(era_method, EXTERNAL_ENCODING);
	strcat(instruction_binary, address);
	strcat(instruction_binary, era_method);
	instruction_binary[WORD_SIZE] = '\0';
	global_state.code_image.code_line[((line_num - 1) * 4) + idx_operand+1] = instruction_binary;
}
bool insert_entry(char* label) {
	/* Find the directive in the line */
	char* ptr = label + strlen(ENTRY_DIRECTIVE);
	SymbolTableEntry* entry = get_symbol(label);
	if (entry == NULL) return false;
	add_to_symbol_table(label, entry->address, entry->relocatable
		, false, ENTRY_DIRECTIVE);
	global_state.entry_counter++;
	return true;
}
int split_line(char* line, char** tokens) {
	int num_tokens = 0;
	char* token = strtok(line, " \t\n");
	while (token != NULL && num_tokens < MAX_LINE_WORDS_COUNT) {
		tokens[num_tokens++] = token;
		token = strtok(NULL, " \t\n");
	}
	return num_tokens;
}