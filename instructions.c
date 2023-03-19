#include "instructions.h"
#include "operand.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// Declare global variables
int memory[MEMORY_SIZE];
MachineState* machine_state_instance = NULL;

/* Private constructor for MachineState*/
MachineState* MachineState_new() {
    MachineState* machine_state = malloc(sizeof(MachineState));
    machine_state->pc = 0;
    machine_state->flags = 0;
    return machine_state;
}

// Public method to get the singleton instance of MachineState
MachineState* get_machine_state() {
    if (machine_state_instance == NULL) {
        machine_state_instance = MachineState_new();
    }
    return machine_state_instance;
}
void set_program_counter(int value) {
    machine_state_instance->pc = value;
}
int get_program_counter() {
    return machine_state_instance->pc;
}
// Reads the value of an operand based on its addressing method
int read_operand_value(Operand operand) {
    switch (operand.addressing_method) {
    case IMMEDIATE:
        return operand.value;
    case DIRECT:
    case REGISTER_DIRECT:
        return memory[operand.address];
    default:
        return 0;
    }
}

/* Writes the value to an operand based on its addressing method*/
void write_operand_value(Operand operand, int value) {
    switch (operand.addressing_method) {
    case DIRECT:
    case REGISTER_DIRECT:
        memory[operand.address] = value;
        break;
    default:
        break;
    }
}

/* Reads the address of an operand based on its addressing method*/
int read_operand_address(Operand operand) {
    switch (operand.addressing_method) {
    case DIRECT:
    case REGISTER_DIRECT:
        return operand.address;
    default:
        return 0;
    }
}

/* Sets the zero flag based on the given value*/
void set_zero_flag(bool value) {
    MachineState* machine_state = get_machine_state();

    if (value) {
        machine_state->flags |= ZERO_FLAG_MASK;
    }
    else {
        machine_state->flags &= ~ZERO_FLAG_MASK;
    }
}
/* Returns the value of the zero flag */
bool get_zero_flag() {
    return (machine_state_instance->flags & ZERO_FLAG_MASK) != 0;
}

/* Pushes the given value onto the stack */
void push_stack(int value) {
    Operand stack_pointer = create_operand(0, machine_state_instance->registers[SP_REG], DIRECT);
    write_operand_value(stack_pointer, value);
    machine_state_instance->registers[SP_REG]--;
}
/**
 * Copies the content of the source operand to the destination operand.
 */
void mov(Operand source, Operand destination) {
    int source_value = read_operand_value(source);
    write_operand_value(destination, source_value);
}

/**
 * Compares the content of the source and destination operands.
 */
void cmp(Operand source, Operand destination) {
    int source_value = read_operand_value(source);
    int destination_value = read_operand_value(destination);

    if (source_value == destination_value) {
        set_zero_flag(true);
    }
    else {
        set_zero_flag(false);
    }
}

/**
 * Adds the content of the source operand to the destination operand.
 */
void add(Operand source, Operand destination) {
    int source_value = read_operand_value(source);
    int destination_value = read_operand_value(destination);

    int result = source_value + destination_value;
    write_operand_value(destination, result);
}

/**
 * Subtracts the content of the source operand from the destination operand.
 */
void sub(Operand source, Operand destination) {
    int source_value = read_operand_value(source);
    int destination_value = read_operand_value(destination);

    int result = destination_value - source_value;
    write_operand_value(destination, result);
}

/**
 * Loads the effective address of the source operand into the destination operand.
 */
void lea(Operand source, Operand destination) {
    int source_address = read_operand_address(source);
    write_operand_value(destination, source_address);
}

/**
 * Inverts the bits of the operand.
 */
void not(Operand operand) {
    int operand_value = read_operand_value(operand);
    write_operand_value(operand, ~operand_value);
}

/**
 * Resets the content of the operand to zero.
 */
void clr(Operand operand) {
    write_operand_value(operand, 0);
}

/**
 * Increments the content of the operand by one.
 */
void inc(Operand operand) {
    int operand_value = read_operand_value(operand);
    write_operand_value(operand, operand_value + 1);
}

/**
 * Decrements the content of the operand by one.
 */
void dec(Operand operand) {
    int operand_value = read_operand_value(operand);
    write_operand_value(operand, operand_value - 1);
}

/**
 * Jumps to the instruction at the given address.
 */
void jmp(Operand operand) {
    int address = read_operand_value(operand);
    set_program_counter(address);
}

/**
 * Jumps to the instruction at the given address if the zero flag is not set.
 */
void bne(Operand operand) {
    if (!get_zero_flag()) {
        int address = read_operand_value(operand);
        set_program_counter(address);
    }
}

/**
 * Reads a value from standard input and stores it in the given operand.
 */
void red(Operand operand) {
    int value;
    scanf("%d", &value);
    write_operand_value(operand, value);
}

/**
 * Prints the value of the given operand to standard output.
 */
void prn(Operand operand) {
    int operand_value = read_operand_value(operand);
    printf("%d\n", operand_value);
}

/**
 * Pushes the current program counter onto the stack and jumps to the given address.
 */
void jsr(Operand operand) {
    push_stack(get_program_counter());
    int address = read_operand_value(operand);
    set_program_counter(address);
}
/**
 * Returns from a subroutine and sets the program counter to the address at the top of the stack.
 */
void rts() {
    int return_address = read_operand_value(create_operand(DIRECT, machine_state_instance->registers[SP_REG], IMMEDIATE));
    machine_state_instance->registers[SP_REG] += 1;
    set_program_counter(return_address);
}
/**
 * Stops the program execution.
 */
void stop(MachineState* machine_state) {
    set_program_counter(MEMORY_SIZE); /* Jump to the end of memory to stop execution*/
}