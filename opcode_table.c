#include "operand.h"
#include "utilities.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
OpcodeTableEntry opcode_table[] = {
	{"mov", 0},
	{"cmp", 1},
	{"add", 2},
	{"sub", 3},
	{"not", 4},
	{"clr", 5},
	{"lea", 6},
	{"inc", 7},
	{"dec", 8},
	{"jmp", 9},
	{"bne", 10},
	{"red", 11},
	{"prn", 12},
	{"jsr", 13},
	{"rts", 14},
	{"stop", 15}
};
 OpcodeTableEntry* get_opcode( char* line, int skip_chars) {
	char instruction[MAX_LABEL_LENGTH];
	OpcodeTableEntry* opcode_entry; 
	if (!parse_instruction(line, instruction, skip_chars)) {
		/*/ No instruction found*/
		return NULL;
	}
	opcode_entry = opcode_table_lookup(instruction);
	if (opcode_entry == NULL) {
		/* Opcode not found in the table*/
		printf("Error: invalid opcode '%s'\n", instruction);
		return NULL;
	}

	return opcode_entry;
}
 OpcodeTableEntry* opcode_table_lookup( char* opcode_name) {
	int i;
	for (i = 0; i < sizeof(opcode_table) / sizeof(OpcodeTableEntry); i++) {
		if (strcmp(opcode_table[i].mnemonic, opcode_name) == 0) {
			return &opcode_table[i];
		}
	}
	return NULL;
}