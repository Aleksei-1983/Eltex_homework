CC = gcc

all: Client

Client: Client.o
	$(CC) -o Client Client.o

Client.o: Client.c
	$(CC) -c Client.c -g 

clean:
	rm -f Client Client.o
