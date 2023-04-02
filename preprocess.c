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
static Macro* macro_table;
void insert_macro( char* name, MacroLine* lines);
Macro* get_macro( char* name);

bool preprocess( char* base_filename) {
    char line[MAX_LINE_LENGTH];
    char macro_name[MAX_LABEL_LENGTH];
    char as_filename[MAX_FILENAME_LENGTH];
    char am_filename[MAX_FILENAME_LENGTH];
    Macro* macro;
    FILE* as_file;
    FILE* am_file;
    sprintf(as_filename, "%s.as", base_filename);
    sprintf(am_filename, "%s.am", base_filename);
    macro_table = NULL;
    as_file = fopen(as_filename, "r");
    if (!as_file) {
        printf("Error: Failed to open input file: %s\n", base_filename);
        return false;
    }
    am_file = fopen(am_filename, "w");
    if (!am_file) {
        printf("Error: Failed to create output file: %s\n", am_filename);
        fclose(as_file);
        return false;
    }

    while (fgets(line, MAX_LINE_LENGTH, as_file)) {
        /* Check for macro definition */
        if (strncmp(line, "mcr", 3) == 0) {
            MacroLine* lines = NULL;
            MacroLine* curr_line = NULL;
            /* Insert macro name into macro table */
            sscanf(line, "mcr %s", macro_name);
            while (fgets(line, MAX_LINE_LENGTH, as_file)) {
                if (strncmp(line, "endmcr", 6) == 0) {
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
            macro = get_macro(macro_name);
            if (macro) {
                /* Copy macro lines to output file */
                MacroLine* curr_line = macro->lines;
                while (curr_line) {
                    fprintf(am_file, "%s", curr_line->line);
                    curr_line = curr_line->next;
                }
            }
            else {
                /* Write line to output file */
                fputs(line, am_file);
            }
        }
    }
    printf("Close am file: %s\n", base_filename);

    /* Close files */
    fclose(as_file);
    printf("Finished closing as file: %s\n", base_filename);
    fclose(am_file);
    printf("Finished closing am file: %s\n", base_filename);
    free(am_file);
    free(as_file);
    return true;
}

void insert_macro( char* name, MacroLine* lines) {
    Macro* new_macro = (Macro*)malloc(sizeof(Macro));
    strncpy(new_macro->name, name, MAX_LABEL_LENGTH);
    new_macro->lines = lines;
    new_macro->next = macro_table;
    macro_table = new_macro;
}

Macro* get_macro( char* name) {
    Macro* curr_macro = macro_table;
    while (curr_macro) {
        if (strcmp(curr_macro->name, name) == 0) {
            return curr_macro;
        }
        curr_macro = curr_macro->next;
    }
    return NULL;
}
