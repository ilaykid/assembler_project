#include <stdio.h>
#include <stdlib.h>
#include "first_pass.h"
#include "second_pass.h"
#include "utilities.h"
#include "output_files.h"
#include "preprocess.h"

AssemblerState global_state;

int main(int argc, char* argv[]) {
    int i = 1;
    if (argc < 2) {
        printf("Usage: %s <assembly_file>\n", argv[0]);
        exit(1);
    }

    /* Process input assembly files one by one */
    for (i = 1; i < argc; i++) {
         char* base_filename = argv[i];
        printf("Processing file: %s\n", base_filename);
        if (!preprocess(base_filename)) {
            printf("Error: Preprocessing failed for file: %s\n", base_filename);
            continue;
        }
        printf("Finished processing file: %s\n", base_filename);
        
        /* First pass */
        if (!first_pass(base_filename)) {
            printf("Error: First pass failed for file: %s\n", base_filename);
            continue;
        }

        /* Second pass */
        if (!second_pass(base_filename)) {
            printf("Error: Second pass failed for file: %s\n", base_filename);
            continue;
        }

        /* Generate output files */
        if (!generate_output_files(base_filename)) {
            printf("Error: Failed to generate output files for file: %s\n", base_filename);
        }
        else {
            printf("Successfully processed file: %s\n", base_filename);
        }
    }

    return 0;
}
