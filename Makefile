CFLAGS = -g

pls: main.o
	gcc $(CFLAGS) -o pls main.o

main.o: main.c
	gcc $(CFLAGS) -c main.c

clean:
	rm -f *.o pls
