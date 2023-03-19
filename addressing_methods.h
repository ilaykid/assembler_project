#ifndef ADDRESSING_METHODS_H
#define ADDRESSING_METHODS_H

#include <stdbool.h>

int encode_immediate(int value);
int encode_direct(int address);
int encode_register_direct(int reg_num, int is_source);

int decode_immediate(int word);
int decode_direct(int word);
int decode_register_direct(int word, int is_source);

#endif // ADDRESSING_METHODS_H
