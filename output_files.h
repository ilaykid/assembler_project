#ifndef OUTPUT_FILES_H
#define OUTPUT_FILES_H

bool generate_output_files(const char* base_filename);

void insert_symbols_to_file(FILE* file, char* directive);

void insert_data_to_obj_file(FILE* obj_file);

void insert_code_to_obj_file(FILE* obj_file);

#endif /* OUTPUT_FILES_H */