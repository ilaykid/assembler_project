#pragma once

#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Maximum length of a line in the input file */
#define MAX_LINE_LENGTH 256
#define MAX_LINE_WORDS_COUNT 6

/* Maximum length of a label in the input file */
#define MAX_LABEL_LENGTH 16

/* Maximum number of macros in the input file */
#define MAX_MACROS 5

/* Maximum length of a string */
#define MAX_STRING_LENGTH 10

/* Maximum number of lines in a macro */
#define MAX_LINES_PER_MACRO 4

/* Maximum length of a symbol in the input file */
#define MAX_SYMBOL_LENGTH 31

/* Maximum length of a filename */
#define MAX_FILENAME_LENGTH 128

/* Maximum number of operands for an instruction */
#define MAX_OPERANDS 3

/* Maximum size of the code and data arrays */
#define MAX_CODE_ARRAY_SIZE 4096
#define MAX_DATA_ARRAY_SIZE 4096

/* Maximum size of an opcode or operand */
#define MAX_OPCODE_LENGTH 10
#define MAX_OPERANDS_LENGTH 20

/* Number of registers on our system*/
#define NUM_REGISTERS 8

/* Register size on our system*/
#define REGISTER_SIZE 14

/* Memory word size */
#define WORD_SIZE 14

/* Varibles and masks for encoding instructions */
#define ERA_BITS 2
#define OPERAND_ADDR_BITS 2
#define OPCODE_BITS 4
#define PARAM_BITS 2
#define ADDRESS_BITS 12

/* Encoding constants */
#define ABSOLUTE_ENCODING ".."
#define RELOCATABLE_ENCODING "./"
#define EXTERNAL_ENCODING "/."

#define MAX_REGISTER_NUM 8

/* Addressing modes */
#define DIRECT 0
#define IMMEDIATE 1
#define JUMP 2
#define REGISTER_DIRECT 3

/* Directives */
#define STRING_DIRECTIVE ".string"
#define DATA_DIRECTIVE ".data"
#define ENTRY_DIRECTIVE ".entry"
#define EXTERN_DIRECTIVE ".extern"

#endif /* CONSTANTS_H */
