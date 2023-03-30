#ifndef OPERAND_H
#define OPERAND_H

#include "constants.h"
#include <stdbool.h>
typedef struct {
    int value;
    int address;
    int addressing_method;
} Operand;
Operand create_operand(int value, int address, int addressing_method);
char* get_operands_string(const char* line, const char* mnemonic_name);
#endif // OPERAND_H
