# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -ansi
LDFLAGS = -lm

# Directories
SRC_DIR = src
OBJ_DIR = obj

# Source files and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/stack/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
DEP_FILES = $(OBJ_FILES:.o=.d)

# Output program
PROGRAM = program

# Default target: build the program
all: $(PROGRAM)

# Rule to create the program
$(PROGRAM): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@ $(LDFLAGS)
	@echo "Built $(PROGRAM)"

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)  # Create obj directories as needed
	$(CC) $(CFLAGS) -MMD -c $< -o $@

# Include dependency files for incremental builds
-include $(DEP_FILES)

# Clean target: remove object files and the program
clean:
	rm -rf $(OBJ_DIR) $(PROGRAM)
	
# Phony targets
.PHONY: all clean
