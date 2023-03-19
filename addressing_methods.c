#include "addressing_methods.h"
#include "constants.h"

/**
 * Encode immediate addressing method (method 0)
 */
int encode_immediate(int value) {
    return (value & MAX_IMMEDIATE_VALUE) | (ABSOLUTE_ENCODING << ARE_SHIFT);
}

/**
 * Encode direct addressing method (method 1)
 */
int encode_direct(int address) {
    return (address & MAX_DIRECT_ADDRESS) | (ABSOLUTE_ENCODING << ARE_SHIFT);
}

/**
 * Encode register direct addressing method (method 3)
 */
int encode_register_direct(int reg_num, int is_source) {
    int encoded_reg = (reg_num & MAX_REGISTER_NUM) << (is_source ? SRC_REG_SHIFT : DEST_REG_SHIFT);
    return encoded_reg | (ABSOLUTE_ENCODING << ARE_SHIFT);
}

/**
 * Decode immediate addressing method (method 0)
 */
int decode_immediate(int word) {
    // Sign extend the 12-bit immediate value to 16-bit
    int value = word & MAX_IMMEDIATE_VALUE;
    if (value & SIGN_BIT_IMMEDIATE) {
        value |= SIGN_EXTEND_IMMEDIATE;
    }
    return value;
}

/**
 * Decode direct addressing method (method 1)
 */
int decode_direct(int word) {
    return word & MAX_DIRECT_ADDRESS;
}

/**
 * Decode register direct addressing method (method 3)
 */
int decode_register_direct(int word, int is_source) {
    return (word >> (is_source ? SRC_REG_SHIFT : DEST_REG_SHIFT)) & MAX_REGISTER_NUM;
}
