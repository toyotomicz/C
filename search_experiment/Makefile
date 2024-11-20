CC = gcc

CFLAGS = -Wall -Wextra -ansi -g
LDFLAGS = $(CFLAGS) -lm

BUILD_DIR = build
BIN = search_experiment

all: clean $(BUILD_DIR) $(BUILD_DIR)/$(BIN)

$(BUILD_DIR)/$(BIN): $(BUILD_DIR)/search_experiment.o $(BUILD_DIR)/searchers.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/search_experiment.o: src/search_experiment.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/searchers.o: src/searchers/searchers.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR):
	mkdir $@

# on Windows use `del /F /Q $(BUILD_DIR)`
clean:
	rm -rf $(BUILD_DIR)
