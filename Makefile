CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = assembler
SOURCES = main.c first_pass.c second_pass.c symbol_table.c utilities.c output_generator.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
