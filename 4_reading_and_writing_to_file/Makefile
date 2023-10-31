CC = gcc

all: main

main: main.o
	$(CC) -o main main.o


main.o: main.c main.h
	$(CC) -c main.c

clean:
	rm -f main main.o
