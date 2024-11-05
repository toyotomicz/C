# Linux Makefile
# makefile

CC=gcc
CFLAGS=-Wall -Wextra -pedantic -ansi
SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin

# Source files
SOURCES=$(wildcard $(SRC_DIR)/*.c)
# Object files
OBJECTS=$(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
# Executable name
EXECUTABLE=$(BIN_DIR)/graph

# Default target
all: $(BIN_DIR) $(OBJ_DIR) $(EXECUTABLE)

# Create directories if they don't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Linking
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(CFLAGS)

# Compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean