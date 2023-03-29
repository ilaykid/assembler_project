#include "operand.h"
#include <stdbool.h>
#include <stdlib.h>

Operand create_operand(int value, int address, int addressing_method){
    Operand operand = {
        .value = value,
        //.address = address,
        .addressing_method = addressing_method
    };
    return operand;
}
/* Extract the operand from the given string */
char* get_operand(const char* line) {
    // Skip over the opcode mnemonic
    const char* mnemonic_end = strchr(line, ' ');
    if (mnemonic_end == NULL) {
        return NULL;
    }

    // Skip over any whitespace between the opcode and operand
    const char* operand_start = mnemonic_end + 1;
    while (*operand_start == ' ' || *operand_start == '\t') {
        operand_start++;
    }

    // Check if there is an operand
    if (*operand_start == '\0') {
        return NULL;
    }

    // Return the operand string
    char* operand = strdup(operand_start);
    if (operand == NULL) {
        printf("Error: Failed to allocate memory for operand\n");
        exit(1);
    }
    return operand;
}
///* Extract the operand from the given string */
//bool get_operand(const char* operand_str, Operand* operand) {
//    // Check for immediate addressing mode
//    if (operand_str[0] == '#') {
//        operand->addressing_method = IMMEDIATE;
//        operand->value = atoi(operand_str + 1);
//        return true;
//    }
//    // Check for direct addressing mode
//    else if (is_valid_label(operand_str)) {
//        operand->addressing_method = DIRECT;
//        strncpy(operand->value, operand_str, MAX_LABEL_LENGTH);
//        return true;
//    }
//    // Check for indirect addressing mode
//    else if (operand_str[0] == '*') {
//        if (is_valid_label(operand_str + 1)) {
//            operand->addressing_method = JUMP;
//            strncpy(operand->value, operand_str + 1, MAX_LABEL_LENGTH);
//            return true;
//        }
//        else {
//            printf("Error: Invalid operand %s\n", operand_str);
//            return false;
//        }
//    }
//    // Check for relative addressing mode
//    else if (is_valid_label(operand_str)) {
//        operand->addressing_method = REGISTER_DIRECT;
//        strncpy(operand->value, operand_str, MAX_LABEL_LENGTH);
//        return true;
//    }
//    else {
//        printf("Error: Invalid operand %s\n", operand_str);
//        return false;
//    }
//}