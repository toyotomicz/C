# Makefile pro Linux/Powershell. Nebude fungovat s Windows CMD.

CC = gcc

CFLAGS = -Wall -Wextra -g
LDFLAGS = $(CFLAGS)

BUILD_DIR = build
BIN = loader.exe

all: clean $(BUILD_DIR) $(BUILD_DIR)/$(BIN)

$(BUILD_DIR)/$(BIN): $(BUILD_DIR)/vector.o $(BUILD_DIR)/person.o $(BUILD_DIR)/loader_examples.o $(BUILD_DIR)/main.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/vector.o: src/vector.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/person.o: src/person.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/loader_examples.o: src/loader_examples.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/main.o: src/main.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR):
	mkdir $@

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(BIN)

# Podle zadání semestrální práce (cíl je možné přidat jako závislost cíle `all`).
install: $(BUILD_DIR)/$(BIN)
	cp $(BUILD_DIR)/$(BIN) ./$(BIN)
