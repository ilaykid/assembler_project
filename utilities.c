#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "utilities.h"
#include "opcode_table.h"
OpcodeTableEntry opcode_table[] = {
    {"opcode1", 1}, // Replace "opcode1" with a real opcode and set the appropriate num_words value
    {"opcode2", 2},
    // Add more opcodes here
};
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

/* Function to split a string using the specified delimiter */
char** split_string(const char* str, const char* delimiter, int* count)
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
        result[i++] = strdup(token);
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
bool parse_label(const char* line, char* label) {
    const char* ptr = line;
    int label_len = 0;

    // Skip leading white spaces
    while (isspace(*ptr)) {
        ptr++;
    }

    // Check if the line starts with a label
    if (!isalpha(*ptr) && *ptr != '_') {
        return false;
    }

    // Copy label characters
    while (isalnum(*ptr) || *ptr == '_') {
        label[label_len++] = *ptr++;
    }

    // Check if the label is followed by a colon
    if (*ptr != ':') {
        return false;
    }

    // Null-terminate the label
    label[label_len] = '\0';

    return true;
}
/* Parses an instruction from a given line */
void parse_instruction(const char* line, char* instruction) {
    const char* start = NULL;
    const char* end = NULL;

    /* Find the start and end of the instruction */
    for (const char* ptr = line; *ptr != '\0'; ptr++) {
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
    }
    else {
        instruction[0] = '\0';
    }
}
const OpcodeTableEntry* opcode_table_lookup(const char* opcode) {
    int i;
    for (i = 0; i < sizeof(opcode_table) / sizeof(OpcodeTableEntry); i++) {
        if (strcmp(opcode_table[i].opcode, opcode) == 0) {
            return &opcode_table[i];
        }
    }
    return NULL;
}