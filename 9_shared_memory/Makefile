CC = gcc

all: Server Client

Server: Server.o 
	$(CC) -o Server Server.o 

Client: Client.o
	$(CC) -o Client Client.o

Server.o: Server.c Server_Client.h
	$(CC) -c Server.c

Client.o: Client.c Server_Client.h
	$(CC) -c Client.c 

clean:
	rm -f Server Client Server.o Client.o
