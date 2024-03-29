#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "utilities.h"
#include "opcode_table.h"

void init_global_state() {
    SymbolTableEntry* curr_entry = global_state.symbol_table;
    int i;
    while (curr_entry != NULL) {
        SymbolTableEntry* next_entry = curr_entry->next;
        strncpy(curr_entry->symbol, "", MAX_SYMBOL_LENGTH);
        curr_entry = next_entry;
    }
    global_state.instruction_counter = 0;
    global_state.data_counter = 0;
    global_state.symbol_table = NULL;
    global_state.instruction_counter = 0;
    global_state.data_counter = 0;
    global_state.entry_counter = 0;
    global_state.extern_counter = 0;
    global_state.code_length = 0;
    global_state.data_length = 0;
    for (i = 0; i < MAX_CODE_ARRAY_SIZE; i++) {
        global_state.code_image.code_line[i] = NULL;
        global_state.data_image.code_line[i] = NULL;
    }
}
bool is_line_contains_word( char* line,  char* word) {
     char* result = strstr(line, word);
    return (result != NULL);
}
/* Function to remove whitespace from the beginning and end of a string */
void trim_whitespace(char* str)
{
    char* end;

    /* Trim leading space */
    while (isspace(*str)) str++;

    /* Trim trailing space */
    end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;

    /* Write new null terminator */
    *(end + 1) = '\0';
}
/* Implement my own strdup*/
char* strdup( char* str)
{
    size_t len = strlen(str);
    char* copy = (char*)malloc(len + 1);
    if (copy != NULL)
    {
        strcpy(copy, str);
    }
    return copy;
}
/* Function to split a string using the specified delimiter */
char** split_string( char* str,  char* delimiter, int* count)
{
    int i = 0;
    char* token;
    char* str_copy = strdup(str);
    char** result = NULL;

    /* Count tokens */
    token = strtok(str_copy, delimiter);
    while (token != NULL)
    {
        i++;
        token = strtok(NULL, delimiter);
    }

    /* Allocate memory for result */
    result = (char**)malloc(i * sizeof(char*));
    if (result == NULL)
    {
        free(str_copy);
        return NULL;
    }

    /* Split string */
    i = 0;
    strcpy(str_copy, str);
    token = strtok(str_copy, delimiter);
    while (token != NULL)
    {
        result[i++] =(char*)strdup(token);
        token = strtok(NULL, delimiter);
    }

    free(str_copy);
    *count = i;
    return result;
}

/* Function to free the memory allocated by split_string */
void free_split_string(char** str_array, int count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        free(str_array[i]);
    }
    free(str_array);
}
/* Function to parse a label from a line of assembly code */
bool parse_label( char* line, char* label) {
     char* ptr = line;
    int label_len = 0;
    /* Skip leading white spaces*/
    while (isspace(*ptr)) {
        ptr++;
    }
    /* Check if the line starts with a label*/
    if (!isalpha(*ptr) && *ptr != '_') {
        return false;
    }

    /*/ Copy label characters*/
    while (isalnum(*ptr) || *ptr == '_') {
        label[label_len++] = *ptr++;
    }
    /*/ Null-terminate the label*/
    label[label_len] = '\0';
    /*/ Check if the label is followed by a colon*/
    if (*ptr != ':') {
        return false;
    }
    return true;
}
/* Parses an instruction from a given line */
bool parse_instruction( char* line, char* instruction,int skip_chars) {
     char* start = NULL;
     char* end = NULL;
     char* ptr;
    /* Find the start and end of the instruction */
    for (ptr = line+(skip_chars*sizeof(char)); *ptr != '\0'; ptr++) {
        if (!isspace((unsigned char)*ptr)) {
            if (start == NULL) {
                start = ptr;
            }
            end = ptr;
        }
        else if (start != NULL) {
            break;
        }
    }

    if (start != NULL && end != NULL) {
        int len = end - start + 1;
        strncpy(instruction, start, len);
        instruction[len] = '\0';
        return true;
    }
    else {
        instruction[0] = '\0';
        return false;
    }
}

/* Function to perform two's complement addition */
int twos_complement_add(int a, int b) {
    int result = a + b;
    int mask = (1 << WORD_SIZE) - 1; /*/ Mask to limit the result to WORD_SIZE bits*/
    return result & mask;
}

/* Function to perform two's complement subtraction */
int twos_complement_subtract(int a, int b) {
    int result = a - b;
    int mask = (1 << WORD_SIZE) - 1; /*/ Mask to limit the result to WORD_SIZE bits*/
    return result & mask;
}
/* Function to check if a number is negative in two's complement representation */
bool is_negative(int number) {
    int mask = 1 << (WORD_SIZE - 1); /*/ Mask to check the most significant bit*/
    return (number & mask) != 0;
}
char* unique_binary_code(int decimal_num) {
    char* binary_code = (char*)malloc((WORD_SIZE + 1) * sizeof(char));
    /*/ Calculate the binary representation of the absolute value*/
    int abs_value = abs(decimal_num);
    int remainder;
    int i = WORD_SIZE - 1;
    if (binary_code == NULL) {
        printf("Error: Failed to allocate memory for binary code\n");
        exit(1);
    }
    while (i >= 0) {
        remainder = abs_value % 2;
        binary_code[i] = (remainder == 0) ? '0' : '1';
        abs_value /= 2;
        i--;
    }
    /*/ Check if the value is negative and apply two's complement if necessary*/
    if (decimal_num < 0) {
        /*/ Flip all the bits*/
        int j;
        for (j = 0; j < WORD_SIZE; j++) {
            binary_code[j] = (binary_code[j] == '0') ? '1' : '0';
        }
        /*/ Add 1 to the result*/
        i = WORD_SIZE - 1;
        while (i >= 0 && binary_code[i] == '1') {
            binary_code[i] = '0';
            i--;
        }
        if (i >= 0) {
            binary_code[i] = '1';
        }
    }
    binary_code[WORD_SIZE] = '\0';
    return binary_code;
}
void fill_string_with_dots(char str[],int length)
{
    int i;
    for (i = 0; i < length; i++)
        str[i] = '.';
}
char* encode_unique_base_2(int num, int count_bits) {
    char* binary_str = unique_binary_code(num);
    int binary_len = strlen(binary_str);
    int i;
    for (i = 0; i < binary_len; i++)
    {
        if (binary_str[i] == '0')
            binary_str[i] = '.';
        else
            binary_str[i] = '/';
    }
    if (binary_len > count_bits) {
        char* result = (char*)malloc(sizeof(char) * (count_bits + 1));
        strncpy(result, &binary_str[binary_len - count_bits], count_bits);
        result[count_bits] = '\0';
        free(binary_str);
        return result;
    }
    else {
        return binary_str;
    }
}