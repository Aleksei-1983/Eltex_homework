CC = gcc

all: Server Client

Server: Server.o functions.o
	$(CC) -o Server Server.o functions.o

Client: Client.o functions.o
	$(CC) -o Client Client.o functions.o

Server.o: Server.c Server_Client.h
	$(CC) -c Server.c

Client.o: Client.c Server_Client.h
	$(CC) -c Client.c 


functions.o: functions.c Server_Client.h
	$(CC) -c functions.c

clean:
	rm -f Server Client Server.o Client.o functions.o
