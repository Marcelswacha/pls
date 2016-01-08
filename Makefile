CFLAGS = -g

SRC = main.c node.c concat.c

OBJ = $(SRC:.c=.o)

all: $(OBJ)
	gcc $(CFLAGS) $(SRC) -o pls

clean:
	rm -f *.o pls
