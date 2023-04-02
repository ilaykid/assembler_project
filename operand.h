#ifndef OPERAND_H
#define OPERAND_H

#include "constants.h"
#include <stdbool.h>
typedef struct {
    int value;
    int address;
    int addressing_method;
} Operand;
bool is_jump_opcode(char* mnemonic_name);
bool is_valid_register( char* reg);
char* get_operands_string( char* line,  char* mnemonic_name);
bool get_operand_object( char* operand_str, Operand* operand, bool is_jump);
int handle_and_count_operands(char* line, int line_number,
    int instruction_counter, char* mnemonic_name, Operand operands[]);
#endif /* OPERAND_H*/
