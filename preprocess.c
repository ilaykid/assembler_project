#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "constants.h"

typedef struct MacroLine {
    char line[MAX_LINE_LENGTH];
    struct MacroLine* next;
} MacroLine;

typedef struct Macro {
    char name[MAX_LABEL_LENGTH];
    MacroLine* lines;
    struct Macro* next;
} Macro;
static Macro* macro_table = NULL;
void insert_macro(const char* name, MacroLine* lines);
Macro* get_macro(const char* name);

bool preprocess(const char* input_filename) {
    FILE* input_file = fopen(input_filename, "r");
    if (!input_file) {
        printf("Error: Failed to open input file: %s\n", input_filename);
        return false;
    }
    char output_filename[MAX_FILENAME_LENGTH];
    char base_input_filename[MAX_FILENAME_LENGTH];
    int len1 = strlen(input_filename);
    strncpy(base_input_filename, input_filename, len1 - 3);
    base_input_filename[len1 - 3] = '\0';
    sprintf(output_filename, "%s.am", base_input_filename);
    FILE* output_file = fopen(output_filename, "w");
    if (!output_file) {
        printf("Error: Failed to create output file: %s\n", output_filename);
        fclose(input_file);
        return false;
    }

    int macro_flag = 0;
    char line[MAX_LINE_LENGTH];
    char macro_name[MAX_LABEL_LENGTH];

    while (fgets(line, MAX_LINE_LENGTH, input_file)) {
        /* Check for macro definition */
        if (strncmp(line, "mcr", 3) == 0) {
            /* Turn on macro flag */
            macro_flag = 1;
            /* Insert macro name into macro table */
            sscanf(line, "mcr %s", macro_name);
            MacroLine* lines = NULL;
            MacroLine* curr_line = NULL;
            while (fgets(line, MAX_LINE_LENGTH, input_file)) {
                if (strncmp(line, "endmcr", 6) == 0) {
                    /* Turn off macro flag */
                    macro_flag = 0;
                    /* Insert macro into macro table */
                    insert_macro(macro_name, lines);
                    break;
                }
                else {
                    /* Add line to macro */
                    MacroLine* new_line = (MacroLine*)malloc(sizeof(MacroLine));
                    strncpy(new_line->line, line, MAX_LINE_LENGTH);
                    new_line->next = NULL;
                    if (!lines) {
                        lines = new_line;
                        curr_line = new_line;
                    }
                    else {
                        curr_line->next = new_line;
                        curr_line = new_line;
                    }
                }
            }
        }
        else {
            char macro_name[MAX_LABEL_LENGTH];
            sscanf(line, "%s", macro_name);
            Macro* macro = get_macro(macro_name);
            if (macro) {
                /* Copy macro lines to output file */
                MacroLine* curr_line = macro->lines;
                while (curr_line) {
                    fprintf(output_file, "%s", curr_line->line);
                    curr_line = curr_line->next;
                }
            }
            else {
                /* Write line to output file */
                fputs(line, output_file);
            }
        }
    }

    /* Close files */
    fclose(input_file);
    fclose(output_file);

    return true;
}

void insert_macro(const char* name, MacroLine* lines) {
    Macro* new_macro = (Macro*)malloc(sizeof(Macro));
    strncpy(new_macro->name, name, MAX_LABEL_LENGTH);
    new_macro->lines = lines;
    new_macro->next = macro_table;
    macro_table = new_macro;
}

Macro* get_macro(const char* name) {
    Macro* curr_macro = macro_table;
    while (curr_macro) {
        if (strcmp(curr_macro->name, name) == 0) {
            return curr_macro;
        }
        curr_macro = curr_macro->next;
    }
    return NULL;
}
