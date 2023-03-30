#ifndef OPERAND_H
#define OPERAND_H

#include "constants.h"
#include <stdbool.h>
typedef struct {
    int value;
    int address;
    int addressing_method;
} Operand;
void free_operands(Operand* operands, int num_operands);
//Operand create_operand(int value, int address, int addressing_method);
bool is_valid_register(const char* reg);
char* get_operands_string(const char* line, const char* mnemonic_name);
bool get_operand_object(const char* operand_str, Operand* operand, bool is_jump);
int handle_and_count_operands(const char* line, int line_number,
    int* instruction_counter, char* mnemonic_name, Operand operands[]);
#endif // OPERAND_H
