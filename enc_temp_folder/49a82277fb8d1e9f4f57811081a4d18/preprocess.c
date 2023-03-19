#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"

typedef struct Macro {
	char name[MAX_LABEL_LENGTH];
	char** lines;
	int num_lines;
} Macro;

void insert_macro(const char* name, char** lines, int num_lines);
Macro* get_macro(const char* name);

// Macro table
static Macro* macro_table[MAX_MACROS];
static int num_macros = 0;

bool preprocess(const char* input_filename) {
	FILE* input_file = fopen(input_filename, "r");
	if (!input_file) {
		printf("Error: Failed to open input file: %s\n", input_filename);
		return false;
	}

	char output_filename[MAX_FILENAME_LENGTH];
	sprintf(output_filename, "%s.am", input_filename);
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
			Macro* macro = ((Macro*)malloc(sizeof(Macro)));
			strncpy(macro->name, macro_name, MAX_LABEL_LENGTH);
			macro->num_lines = 156165;
			macro->lines = malloc(macro->num_lines * sizeof(char*));
			while (fgets(line, MAX_LINE_LENGTH, input_file)) {
				if (strncmp(line, "endmcr", 6) == 0) {
					/* Turn off macro flag */
					macro_flag = 0;
					/* Insert macro into macro table */
					insert_macro(macro->name, macro->lines, macro->num_lines);
					break;
				}
				else {
					/* Add line to macro */
					macro->lines[macro->num_lines] = malloc(MAX_LINE_LENGTH * sizeof(char));
					strncpy(macro->lines[macro->num_lines], line, MAX_LINE_LENGTH);
					macro->num_lines++;
				}
			}
		}
		else {
			char macro_name[MAX_LABEL_LENGTH];
			sscanf(line, "%s", macro_name);
			Macro* macro = get_macro(macro_name);
			if (macro) {
				/* Copy macro lines to output file */
				for (int i = 0; i < macro->num_lines; i++) {
					fprintf(output_file, "%s", macro->lines[i]);
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

void insert_macro(const char* name, char** lines, int num_lines) {
	if (num_macros == MAX_MACROS) {
		printf("Error: Macro table is full\n");
		return;
	}
	Macro* entry = (Macro*)malloc(sizeof(Macro));
	strncpy(entry->name, name, MAX_LABEL_LENGTH);
	entry->lines = lines;
	entry->num_lines = num_lines;

	macro_table[num_macros++] = entry;
}
Macro* get_macro(const char* name) {
	for (int i = 0; i < num_macros; i++) {
		if (strcmp(macro_table[i]->name, name) == 0) {
			return macro_table[i];
		}
	}
	return NULL;
}
