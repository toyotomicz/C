# Kompilátor a příznaky
CC = gcc
CFLAGS = -Wall -Wextra -ansi

# Cílový název programu
TARGET = bin/program

# Zdrojové soubory
SRCDIR = src
SRCS = $(SRCDIR)/main.c $(SRCDIR)/parser.c
OBJS = $(SRCS:.c=.o)

# Pravidlo pro vytvoření cílového programu
$(TARGET): $(OBJS) | bin
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

# Vytvoření bin adresáře pokud neexistuje
bin:
	mkdir -p bin

# Pravidlo pro kompilaci jednotlivých .c souborů
$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Vyčištění generovaných souborů
clean:
	rm -f $(OBJS) $(TARGET)
	rmdir bin 2>/dev/null || true