CC = gcc #compiler
OBJS = tree.o bst.o 
BIN = tree
CCFLAGS = -Wall -Wextra -pedantic

all: clean $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CCFLAGS) tree.o bst.o -o tree
tree.o: tree.c
	$(CC) $(CCFLAGS) -c tree.c 

bst.o: vst.c 
	$(CC) $(CCFLAGS) -c bst.c

clean:
	rm -f $(OBJS) $(BIN)