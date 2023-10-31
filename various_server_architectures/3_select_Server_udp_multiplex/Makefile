CC = gcc

all: Server

Server: Server.o
	$(CC) -o Server Server.o

Server.o: Server.c 
	$(CC) -c -g Server.c

clean:
	rm -f Server Server.o
