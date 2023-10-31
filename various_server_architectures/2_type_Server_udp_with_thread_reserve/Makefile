CC = gcc

all: Server Client

Server: Server.o 
	$(CC) -o Server Server.o

Client: Client.o
	$(CC) -o Client Client.o

Server.o: Server.c
	$(CC) -c Server.c -g

Client.o: Client.c
	$(CC) -c Client.c -g 

clean:
	rm -f Server Client Server.o Client.o
