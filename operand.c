#include "operand.h"
#include "utilities.h"
#include <stdbool.h>
#include <stdlib.h>
bool is_valid_register(const char* reg);
char* list_of_registers[MAX_REGISTER_NUM] = 
{"r0","r1","r2" ,"r3" ,"r4" ,"r5" ,"r6" ,"r7" };
Operand create_operand(int value, int address, int addressing_method){
    Operand operand = {
        .value = value,
        //.address = address,
        .addressing_method = addressing_method
    };
    return operand;
}
void free_operands(Operand* operands, int num_operands) {
    for (int i = 0; i < num_operands; i++) {
        if (operands[i].value != NULL) {
            free(operands[i].value);
        }
    }
}
/* Extract the operand from the given string */
char* get_operands_string(const char* line, const char* mnemonic_name) {
    // Check that the line begins with the expected mnemonic name
    if (strncmp(line, mnemonic_name, strlen(mnemonic_name)) != 0) {
        return NULL;
    }

    // Skip over the mnemonic name
    const char* operand_start = line + strlen(mnemonic_name);
    int operand_length = line+strlen(line) - operand_start;
    while (*operand_start == ' ' || *operand_start == '\t') {
        operand_start++;
    }

    // Check if there is an operand
    if (*operand_start == '\0') {
        return NULL;
    }

    // Return the operand string
    char* operands_string = malloc(operand_length + 1);
    strncpy(operands_string, operand_start, operand_length);
    if (operands_string == NULL) {
        printf("Error: Failed to allocate memory for operand\n");
        exit(1);
    }
    operands_string[operand_length] = '\0';
    return operands_string;
}

/* Extract the operand from the given string */
bool get_operand_object(const char* operand_str, Operand* operand,bool is_jump) {
     //Check for immediate addressing mode
    trim_whitespace(operand_str);
    if (operand_str[0] == '#') {
        operand->addressing_method = IMMEDIATE;
        operand->value = atoi(operand_str + 1);
        return true;
    }
    else if (is_valid_register(operand_str)) {
        operand->addressing_method = REGISTER_DIRECT;

        char* reg_value_str = operand_str + 1;
        int reg_value = *reg_value_str - '0';
        operand->value = reg_value;
        //sprintf(operand->value, "%d", reg_value);
        return true;
    }
    // Check for direct addressing mode
    else if(!is_jump)//if (is_valid_label(operand_str)) 
    {
        operand->addressing_method = DIRECT;
        SymbolTableEntry* symbol = get_symbol(operand_str);
        if (symbol==NULL)
            return true;
        else
        {
            operand->value = symbol->address;
        }
        //strncpy(operand->value, operand_str, MAX_LABEL_LENGTH);
    }
    else
    {
        operand->addressing_method = JUMP;
        //strncpy(operand->value, operand_str, MAX_LABEL_LENGTH);
        return true;
    }
    //// Check for indirect addressing mode
    //else if (operand_str[0] == '*') {
    //    if (is_valid_label(operand_str + 1)) {
    //        operand->addressing_method = JUMP;
    //        strncpy(operand->value, operand_str + 1, MAX_LABEL_LENGTH);
    //        return true;
    //    }
    //    else {
    //        printf("Error: Invalid operand %s\n", operand_str);
    //        return false;
    //    }
    //}
    //// Check for relative addressing mode
    //else if (is_valid_label(operand_str)) {
    //    operand->addressing_method = REGISTER_DIRECT;
    //    strncpy(operand->value, operand_str, MAX_LABEL_LENGTH);
    //    return true;
    //}
    //else {
    //    printf("Error: Invalid operand %s\n", operand_str);
    //    return false;
    //}
    return true;//temp
}
bool is_valid_register(const char* reg) {
    for (int i = 0; i < MAX_REGISTER_NUM; i++)
    {
        if (strcmp(reg, list_of_registers[i]) == 0)
        {
            return true;
        }
    }
    return false;
}
bool is_jump_opcode(char* mnemonic_name)
{
    return !strcmp(mnemonic_name, "jmp") || 
        !strcmp(mnemonic_name, "bne") || 
        !strcmp(mnemonic_name, "jsr");
}
/* Process an instruction from the given line */
int handle_and_count_operands(const char* line, int line_number,
    int* instruction_counter, char* mnemonic_name, Operand operands[]) {
    char instruction[MAX_LINE_LENGTH + 1];
    strncpy(instruction, line, MAX_LINE_LENGTH);
    bool is_jump = false;
    if (is_jump_opcode(mnemonic_name))
        is_jump = true;
    //trim_whitespace(instruction);

    // Get the operand string and parse it into operand objects
    char* operand_string = get_operands_string(instruction, mnemonic_name);
    int num_operands = 0;
    int i = 0;
    if (operand_string != NULL) {
        if (is_jump)
        {
            int i = 0;
            char* end_address = operand_string + strlen(operand_string);
            char* ptr = operand_string;
            char jump_label[MAX_LABEL_LENGTH];
            num_operands++;
            while (*ptr != '('&& ptr!= end_address)
            {
                jump_label[i++] = *ptr++;
            }
            jump_label[i] = '\0';
            if (!get_operand_object(jump_label, &operands[0], is_jump)) {
                printf("Error at line %d: Failed to parse operand\n", line_number);
                exit(1);
            }
            if (ptr != end_address) /*there is more registers*/
            {
                ptr++;
                i = 0;
                char jump_reg[MAX_LABEL_LENGTH];
                while (*ptr != ',' && ptr != end_address)
                {
                    jump_reg[i++] = *ptr++;
                }
                jump_reg[i] = '\0';
                num_operands++;
                if (!get_operand_object(jump_reg, &operands[1], is_jump)) {
                    printf("Error at line %d: Failed to parse operand\n", line_number);
                    exit(1);
                }
                ptr++;
                i = 0;
                jump_reg[0]='\0';
                while (*ptr != ')'&& ptr != end_address)
                {
                    jump_reg[i++] = *ptr++;
                }
                jump_reg[i] = '\0';
                num_operands++;
                if (!get_operand_object(jump_reg, &operands[2], is_jump)) {
                    printf("Error at line %d: Failed to parse operand\n", line_number);
                    exit(1);
                }
                if (*ptr != ')')//error
                {
                    printf("Error at line %d: Failed to parse operand\n", line_number);
                    exit(1);
                }
            }
        }
        else {
            char** operand_strings = split_string(operand_string, ",", &num_operands);
            for (i = 0; i < num_operands; i++) {
                if (!get_operand_object(operand_strings[i], &operands[i], is_jump)) {
                    printf("Error at line %d: Failed to parse operand %d\n", line_number, i + 1);
                    exit(1);
                }
            }
            free_split_string(operand_strings, num_operands);
        }
    }
    free(operand_string);
    return num_operands;
}