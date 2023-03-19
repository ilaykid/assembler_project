#ifndef OPERAND_H
#define OPERAND_H

#include "constants.h"
typedef struct {
    int value;
    int address;
    int addressing_method;
} Operand;
Operand create_operand(int value, int address, int addressing_method);



#endif // OPERAND_H
