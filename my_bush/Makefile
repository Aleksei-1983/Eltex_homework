CC = gcc

all: bush

bush: bush.o
	$(CC) -o bush bush.o


bush.o: bush.c bush.h
	$(CC) -c bush.c

clean:
	rm -f bush bush.o
