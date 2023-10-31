CC = gcc

all: Server Client

Server: Server.o read_line.o
	$(CC) -o Server Server.o read_line.o 
 
Client: Client.o read_line.o 
	$(CC) -o Client Client.o read_line.o 

Server.o: Server.c Server_Client.h
	$(CC) -c -g Server.c

Client.o: Client.c Server_Client.h
	$(CC) -c -g Client.c 


read_line.o: read_line.c read_line.h
	$(CC) -c -g read_line.c

clean:
	rm -f Server Client Server.o Client.o read_line.o
