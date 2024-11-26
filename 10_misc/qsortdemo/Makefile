# Makefile pro Linux/Powershell. Nebude fungovat s Windows CMD.

CC = gcc

CFLAGS = -Wall -Wextra
LDFLAGS = $(CFLAGS)

BUILD_DIR = build

$(BUILD_DIR)/stack_size.exe: $(BUILD_DIR) misc/stack_size.c
	$(CC) $(LDFLAGS) -o $@ misc/stack_size.c

$(BUILD_DIR)/stack_size.s: $(BUILD_DIR) misc/stack_size.c
	$(CC) $(LDFLAGS) -o $@ misc/stack_size.c

$(BUILD_DIR)/qsortdemo.exe: $(BUILD_DIR) src/qsortdemo.c
	$(CC) $(LDFLAGS) -o $@ src/qsortdemo.c

$(BUILD_DIR)/qsortdemo_asm.exe: $(BUILD_DIR) $(BUILD_DIR)/qsortdemo.o $(BUILD_DIR)/swap_att_gcc_x86_64.o
	$(CC) $(LDFLAGS) -o $@ $(BUILD_DIR)/qsortdemo.o $(BUILD_DIR)/swap_att_gcc_x86_64.o

$(BUILD_DIR)/qsortdemo.o: $(BUILD_DIR) src/qsortdemo.c
	$(CC) -c $(CFLAGS) -o $@ src/qsortdemo.c

$(BUILD_DIR)/swap_att_gcc_x86_64.o: $(BUILD_DIR) src/swap_att_gcc_x86_64.s
	$(CC) -c $(CFLAGS) -o $@ src/swap_att_gcc_x86_64.s

$(BUILD_DIR):
	mkdir $@

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(BIN)

# Podle zadání semestrální práce (cíl je možné přidat jako závislost cíle `all`).
install: $(BUILD_DIR)/$(BIN)
	cp $(BUILD_DIR)/$(BIN) ./$(BIN)
