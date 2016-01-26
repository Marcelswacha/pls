CFLAGS = -pg -Wall -O3 -std=gnu99

SRC = main.c stack.c node.c concat.c

OBJ = $(SRC:.c=.o)

all: $(OBJ)
	gcc $(CFLAGS) $(SRC) -o pls

clean:
	rm -f *.o pls
