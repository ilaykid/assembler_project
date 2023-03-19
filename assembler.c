#include <stdio.h>
#include <stdlib.h>
#include "first_pass.h"
#include "second_pass.h"
#include "output_generator.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <assembly_file>\n", argv[0]);
        exit(1);
    }

    // Process input assembly files one by one
    for (int i = 1; i < argc; i++) {
        const char* input_filename = argv[i];
        printf("Processing file: %s\n", input_filename);

        if (!preprocess(input_filename)) {
            printf("Error: Preprocessing failed for file: %s\n", input_filename);
            continue;
        }
        // First pass
        if (!first_pass(input_filename)) {
            printf("Error: First pass failed for file: %s\n", input_filename);
            continue;
        }

        // Second pass
        if (!second_pass(input_filename)) {
            printf("Error: Second pass failed for file: %s\n", input_filename);
            continue;
        }

        // Generate output files
        if (!generate_output_files(input_filename)) {
            printf("Error: Failed to generate output files for file: %s\n", input_filename);
        }
        else {
            printf("Successfully processed file: %s\n", input_filename);
        }
    }

    return 0;
}
