#ifndef PIPES_NAME_FIFO
#define PIPES_NAME_FIFO

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>


#define SERVER_FIFO "/tmp/seqnum_sv" /*имя серверной очереди */ 
#define CLIENT_FIFO_TEMPLATE "/tmp/seqnum_cl.%ld" /*шаблон клиенской очереди */
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 20)

struct request {     /*Запрос (клиент --> сервер) */
  pid_t pid;         /*PID клиета */
  int seqLen;        /*Длинна запрашиваемой последовательности */

};

struct response{     /*Ответ (сервер-->  клиент) */
  int seqNum;        /*Начало последовательности */
};

#endif // PIPES_NAME_FIFO
