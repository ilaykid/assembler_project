#include "operand.h"

Operand create_operand(int value, int address, int addressing_method){
    Operand operand = {
        .value = value,
        .address = address,
        .addressing_method = addressing_method
    };
    return operand;
}