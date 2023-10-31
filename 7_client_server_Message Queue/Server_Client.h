#ifndef SERVER_CLIENT
#define SERVER_CLIENT


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>


#define MAX_MSG_SIZE 1024
#define SIZE_STR 50


struct magDate { /* Данные */
  pid_t pid;
  char mtext[MAX_MSG_SIZE];
};

struct msgBuf { /* Структура сообщения*/
  long mtype;
  struct magDate date;
};

/* Функция преобразует число в строку и формирует сообщение
 * строка входная
 * строка входная
 * индекс выходной строки*/
void requestStr(char* reqest, char* str, int index); 
  
#endif // SERVER_CLIRN
