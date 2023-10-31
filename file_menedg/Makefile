CC = gcc
BB = -lncurses
all: file_manager

file_manager: file_manager.o functions.o
	$(CC) -o file_manager file_manager.o functions.o $(BB)

file_manager.o: file_manager.c file_manager.h
	$(CC) $(BB) -c file_manager.c


functions.o: functions.c file_manager.h
	$(CC) $(BB) -c functions.c

clean:
	rm -f file_manager file_manager.o functions.o
