CC = gcc

CFLAGS = -Wall -Wextra -pedantic
LDFLAGS = $(CFLAGS)

BUILD_DIR = build
BIN = steganodecoder.exe

all: clean $(BUILD_DIR) $(BUILD_DIR)/$(BIN)

$(BUILD_DIR)/$(BIN): $(BUILD_DIR)/decoder.o $(BUILD_DIR)/imgio.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/decoder.o: src/decoder.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/imgio.o: src/imgio.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR):
	mkdir $@

# on Linux use    rm -rf $(BUILD_DIR)
# on Windows use  del /F /Q $(BUILD_DIR)  
clean:
	rm -rf $(BUILD_DIR)
# del /F /Q $(BUILD_DIR)