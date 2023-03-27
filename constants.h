#pragma once

#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Maximum length of a line in the input file */
#define MAX_LINE_LENGTH 256
/* Maximum length of a label in the input file */
#define MAX_LABEL_LENGTH 16
/* Maximum number of macros in the input file */
#define MAX_MACROS 5
#define MAX_STRING_LENGTH 10
#define MAX_LINES_PER_MACRO 4
/* Maximum length of a symbol in the input file */
#define MAX_SYMBOL_LENGTH 31

/* Maximum length of a filename */
#define MAX_FILENAME_LENGTH 128

/* Maximum number of operands for an instruction */
#define MAX_OPERANDS 2

/* Maximum size of the code array */
#define MAX_CODE_ARRAY_SIZE 4096
#define MAX_DATA_ARRAY_SIZE 4096

/* Maximum size of opcode */
#define MAX_OPCODE_LENGTH 10
/* Maximum size of opcode */
#define MAX_OPERANDS_LENGTH 20

/* Number of registers on our system*/
#define NUM_REGISTERS 8

/*Register on our system*/
#define REGISTER_SIZE 14

#define MEMORY_SIZE 256

#define WORD_SIZE 14
/* Varibles and masks for encode instuctions */
#define OPCODE_SHIFT 6
#define OPCODE_MASK 0xF
#define SRC_ADDR_SHIFT 4
#define DST_ADDR_SHIFT 2
#define ADDR_METHOD_MASK 0x3
#define PARAM1_SHIFT 12
#define PARAM2_SHIFT 10
#define PARAM_MASK 0x3
#define ENCODING_TYPE_MASK 0x3

#define ZERO_FLAG_MASK 0x1
#define SP_REG 6

/* Encoding constants */
#define ABSOLUTE_ENCODING 0
#define RELOCATABLE_ENCODING 2
#define EXTERNAL_ENCODING 1
#define ARE_SHIFT 10

/* Addressing method constants */
#define MAX_IMMEDIATE_VALUE 0x7FF
#define SIGN_BIT_IMMEDIATE 0x800
#define SIGN_EXTEND_IMMEDIATE 0xFFFFF000
#define MAX_DIRECT_ADDRESS 0xFFF
#define MAX_REGISTER_NUM 7
#define SRC_REG_SHIFT 8
#define DEST_REG_SHIFT 2

#define DIRECT 0
#define IMMEDIATE 1
#define JUMP 2
#define REGISTER_DIRECT 3

#define STRING_DIRECTIVE ".string"
#define DATA_DIRECTIVE ".data"
#define ENTRY_DIRECTIVE ".entry"
#define EXTERN_DIRECTIVE ".extern"


#endif /* CONSTANTS_H */