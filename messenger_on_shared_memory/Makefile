CC = gcc
BB = -lncurses

all: Server Client

Server: Server.o
	$(CC) -o Server Server.o

Client: Client.o functions.o
	$(CC) -o Client Client.o functions.o $(BB)

Server.o: Server.c Server.h
	$(CC) -c Server.c

Client.o: Client.c Client.h
	$(CC) -c Client.c 


functions.o: functions.c Client.h
	$(CC) -c functions.c

clean:
	rm -f Server Client Server.o Client.o functions.o
