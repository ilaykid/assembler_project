=====================================================================
                    ASSEMBLER PROJECT - README.txt
=====================================================================

Introduction:
-------------
This project is a simple assembler for a custom assembly language. The assembler takes assembly source code files as input and generates machine code output files. The project follows a two-pass assembly process, processing labels and instructions to generate the final machine code.

Prerequisites:
--------------
1. C compiler (e.g., GCC, Clang, or MSVC)
2. CMake build system (version 3.8 or later)

Building the Project:
---------------------
1. Navigate to the project directory.
2. Create a new directory called 'build': mkdir build
3. Change into the 'build' directory: cd build
4. Run CMake to generate the build system: cmake ..
5. Build the project using the generated build system:
   - On Linux or macOS: make
   - On Windows with Visual Studio: Open the generated solution file and build the project.

Usage:
------
assembler <input_file.asm>

- <input_file.asm> is the assembly source code file that you want to assemble.

The assembler will generate the following output files in the same directory as the input file:
1. <input_file>.ob: Object file containing the machine code
2. <input_file>.ext: File containing information about external symbols
3. <input_file>.ent: File containing information about entry symbols

For example, if the input file is named 'example.asm', the assembler will generate 'example.ob', 'example.ext', and 'example.ent' files.

Project Structure:
------------------
- main.c: Contains the main function that orchestrates the assembly process.
- first_pass.h & first_pass.c: Implement the first pass of the assembler.
- second_pass.h & second_pass.c: Implement the second pass of the assembler.
- output_generator.h & output_generator.c: Generate the output files (object, external, and entry files).
- symbol_table.h & symbol_table.c: Handle the symbol table and related operations.
- utilities.h & utilities.c: Provide utility functions for parsing and processing instructions.
- constants.h: Define the constants used throughout the project.
- CMakeLists.txt: CMake build configuration file.

Note:
-----
Please refer to the provided documentation for more information about the assembly language syntax, opcodes, and addressing modes.

This project is a starting point and may require further customization and improvements to meet specific requirements. It is designed to be a basic example of an assembler for educational purposes.