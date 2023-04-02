#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <stdbool.h>

/**
 * Performs the preprocessing stage of the assembly process.
 * Modifies the input file to expand macros.
 *
 * @param input_filename The name of the input assembly file.
 * @return true if the preprocessing stage completed successfully, false otherwise.
 */
bool preprocess( char* base_filename);

#endif