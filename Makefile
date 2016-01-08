CFLAGS = -g

pls: main.o
	gcc $(CFLAGS) -o pls main.o

main.o: main.c
	gcc $(CFLAGS) -c main.c node.c

all:
	gcc $(CFLAGS) -o pls main.o

clean:
	rm -f *.o pls
