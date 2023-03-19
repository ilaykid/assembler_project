#include "instruction_structure.h"
#include "constants.h"
#include <string.h>
#include <stdlib.h>

/* Function to convert a single character of unique base 2 to its binary representation */
int char_to_binary(char c) {
    if (c == '.') {
        return 0;
    }
    else if (c == '/') {
        return 1;
    }
    else {
        return -1;
    }
}

/* Function to convert a single binary digit to its unique base 2 representation */
char binary_to_char(int binary) {
    return binary == 0 ? '.' : '/';
}

/* Function to decode the first word of a machine instruction */
MachineInstruction decode_instruction(const char* first_word) {
    MachineInstruction instruction;
    int binary_first_word = 0;

    for (int i = 0; i < strlen(first_word); i++) {
        binary_first_word = (binary_first_word << 1) | char_to_binary(first_word[i]);
    }

    instruction.opcode = (binary_first_word >> OPCODE_SHIFT) & OPCODE_MASK;
    instruction.src_addressing = (binary_first_word >> SRC_ADDR_SHIFT) & ADDR_METHOD_MASK;
    instruction.dst_addressing = (binary_first_word >> DST_ADDR_SHIFT) & ADDR_METHOD_MASK;
    instruction.param1 = (binary_first_word >> PARAM1_SHIFT) & PARAM_MASK;
    instruction.param2 = (binary_first_word >> PARAM2_SHIFT) & PARAM_MASK;
    instruction.encoding_type = binary_first_word & ENCODING_TYPE_MASK;

    return instruction;
}

/* Function to encode a machine instruction into its first word */
char* encode_instruction(MachineInstruction instruction) {
    int binary_first_word = 0;
    char* first_word = (char*)malloc((WORD_SIZE + 1) * sizeof(char));

    binary_first_word |= (instruction.opcode & OPCODE_MASK) << OPCODE_SHIFT;
    binary_first_word |= (instruction.src_addressing & ADDR_METHOD_MASK) << SRC_ADDR_SHIFT;
    binary_first_word |= (instruction.dst_addressing & ADDR_METHOD_MASK) << DST_ADDR_SHIFT;
    binary_first_word |= (instruction.param1 & PARAM_MASK) << PARAM1_SHIFT;
    binary_first_word |= (instruction.param2 & PARAM_MASK) << PARAM2_SHIFT;
    binary_first_word |= instruction.encoding_type & ENCODING_TYPE_MASK;

    for (int i = WORD_SIZE - 1; i >= 0; i--) {
        first_word[WORD_SIZE - 1 - i] = binary_to_char((binary_first_word >> i) & 1);
    }

    first_word[WORD_SIZE] = '\0';

    return first_word;
}