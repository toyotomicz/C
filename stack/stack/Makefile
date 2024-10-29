CC = gcc

CFLAGS = -Wall -Wextra -pedantic -ansi -g
LDFLAGS = $(CFLAGS)

BUILD_DIR = build
BIN = parser

all: clean $(BUILD_DIR) $(BUILD_DIR)/$(BIN)

$(BUILD_DIR)/$(BIN): $(BUILD_DIR)/parser.o $(BUILD_DIR)/stack.o $(BUILD_DIR)/operators.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/parser.o: src/parser.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/operators.o: src/operators.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/stack.o: src/stack/stack.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR):
	mkdir $@

# on Windows use del /F /Q $(BUILD_DIR)
clean:
	rm -rf $(BUILD_DIR)
