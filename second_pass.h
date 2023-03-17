#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <stdbool.h>

// Constants for the maximum length of labels, opcodes, and operands
#define MAX_LABEL_LENGTH 31
#define MAX_OPCODE_LENGTH 10
#define MAX_OPERANDS_LENGTH 100

// Function prototype for process_line_second_pass
bool process_line_second_pass(const char* line);

#endif // SECOND_PASS_H
