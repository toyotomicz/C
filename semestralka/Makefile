# Cesta k složkám
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Název výsledného spustitelného souboru
TARGET = $(BIN_DIR)/program

# Kompilátor a příznaky
CC = gcc
CFLAGS = -Wall -I$(SRC_DIR) -I$(SRC_DIR)/stack

# Seznam zdrojových souborů
SRCS = $(SRC_DIR)/main2.c \
       $(SRC_DIR)/parser2.c \
       $(SRC_DIR)/postscript.c \
       $(SRC_DIR)/stack/stack.c

# Vytvoření seznamu objektových souborů
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Výchozí cíl
all: $(TARGET)

# Pravidlo pro vytvoření spustitelného souboru
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

# Pravidlo pro vytvoření objektových souborů
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/stack
	$(CC) $(CFLAGS) -MMD -c $< -o $@

# Zahrnutí závislostí vytvořených pomocí -MMD
-include $(OBJS:.o=.d)

# Pravidlo pro vyčištění objektových a binárních souborů
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)/program

# Pravidlo pro vyčištění a přeložení znovu
rebuild: clean all

.PHONY: all clean rebuild
