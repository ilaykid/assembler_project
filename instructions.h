#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include "constants.h"
#include "addressing_methods.h"
#include "operand.h"

void mov(Operand source, Operand destination);
void cmp(Operand source, Operand destination);
void add(Operand source, Operand destination);
void sub(Operand source, Operand destination);
void lea(Operand source, Operand destination);
/* functions declarations */
int execute_next_instruction();
int read_operand_value(Operand operand);
int read_operand_address(Operand operand);
void write_operand_value(Operand operand, int value);
void set_zero_flag(int value);
typedef struct {
    int mnemonic;
    char operands[MAX_OPERANDS_LENGTH + 1];
} Instruction;

/* MachineState Declarations */
typedef struct {
    int memory[MEMORY_SIZE];
    int registers[NUM_REGISTERS];
    int pc;
    int flags;
} MachineState;

#endif /* INSTRUCTION_H */