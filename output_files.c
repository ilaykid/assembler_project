#include <stdbool.h>
#include <stdio.h>
#include "constants.h"
#include "UTILITIES.H"
#include "output_files.h"

bool generate_output_files(char* base_filename)
{
    AssemblerState global_state;
	char obj_filename[MAX_FILENAME_LENGTH];
	sprintf(obj_filename, "%s.ob", base_filename);
    FILE* obj_file = fopen(obj_filename, "w");
    if (!obj_file) {
        printf("Error: Failed to create output file: %s\n", obj_filename);
        fclose(obj_file);
        return false;
    }
    for (int i = 0; i < strlen(global_state.code_image); i++)
    {
        char* line = global_state.code_image.code_line[i];
        if (line != NULL)
        {
            /* Write line to output file */
            fputs(line, obj_file);
        }
    }
	return true;
}