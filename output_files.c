#include <stdbool.h>
#include <stdio.h>
#include "constants.h"
#include "UTILITIES.H"
#include "output_files.h"

bool generate_output_files(char* base_filename)
{
	char obj_filename[MAX_FILENAME_LENGTH];
	char entry_filename[MAX_FILENAME_LENGTH];
	char extern_filename[MAX_FILENAME_LENGTH];
	sprintf(obj_filename, "%s.ob", base_filename);
	sprintf(entry_filename, "%s.ent", base_filename);
	sprintf(extern_filename, "%s.ext", base_filename);
    FILE* obj_file = fopen(obj_filename, "w");
    if (!obj_file) {
        printf("Error: Failed to create output file: %s\n", obj_filename);
        fclose(obj_file);
        return false;
    }
    insert_code_to_obj_file(obj_file);
    insert_data_to_obj_file(obj_file);
    fclose(obj_file);
    if (global_state.entry_counter > 0)
    {
        FILE* ent_file = fopen(entry_filename, "w");
        insert_symbols_to_file(ent_file, ENTRY_DIRECTIVE);
        fclose(ent_file);
    }
    if (global_state.extern_counter > 0)
    {
        FILE* ext_file = fopen(extern_filename, "w");
        insert_symbols_to_file(ext_file, EXTERN_DIRECTIVE);
        fclose(ext_file);
    }
	return true;
}

void insert_symbols_to_file(FILE* file,char* directive)
{
    SymbolTableEntry* entry = global_state.symbol_table;

    while (entry != NULL) {
        // Compare the symbol name with the extern's symbol
        if (strcmp(entry->symbol_type, directive) == 0) {
            char* line = entry->symbol;
            strcat(line, "\t");
            strcat(line, encode_unique_base_2(entry->address, WORD_SIZE));
            strcat(line, "\n");
            fputs(line, file);
        }
        entry = entry->next; // move to the next entry in the linked list
    }
}

void insert_data_to_obj_file(FILE* obj_file)
{
    for (int i = 0; i < global_state.data_counter; i++)
    {
        char* line = global_state.data_image.code_line[i];
        if (line)
            if (line[0] == '.' || line[0] == '/')
            {
                /* Write line to output file */
                strcat(line, "\n");
                fputs(line, obj_file);
            }
    }
}

void insert_code_to_obj_file(FILE* obj_file)
{
    for (int i = 0; i < global_state.instruction_counter * 4; i++)
    {
        char* line = global_state.code_image.code_line[i];
        if (line)
            if (line[0] == '.' || line[0] == '/')
            {
                /* Write line to output file */
                strcat(line,"\n");
                fputs(line, obj_file);
            }
    }
}
