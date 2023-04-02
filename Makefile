CC = gcc

CFLAGS = -pedantic -ansi -Wall  

TARGET = assembler

SOURCES = assembler.c first_pass.c second_pass.c symbol_table.c utilities.c output_files.c preprocess.c operand.c opcode_table.c

OBJECTS = $(SOURCES:.c=.o)



all: $(TARGET)



$(TARGET): $(OBJECTS)

	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)



%.o: %.c

	$(CC) $(CFLAGS) -c $< -o $@



clean:

	rm -f $(TARGET) $(OBJECTS)

