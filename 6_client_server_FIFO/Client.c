/*Клиент при запуске программы в аргументе записываем число на которое надо увеличить счетчик сервера.
 * С начало получаем его а потом увеличиваем */
#include "pipes_name_fifo.h"


static char clientFifo[CLIENT_FIFO_NAME_LEN];

/* Вызывается при выходе для удаления клиенской очереди FIFO*/
static void removeFifo(void)
{
  unlink(clientFifo);
}

int main(int argc, char** argv)
{
  int serverFd;
  int clientFd;
  struct request req;
  struct response resp;

  umask(0);
  
  /*Создает нашу очередь FIFO (до отправки звпроса, чтобы избежать состояние гонки)*/
  snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, (long) getpid() );
  
  if(mkfifo(clientFifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST){
    printf("mkfifo %s\n", clientFifo);
    exit(EXIT_FAILURE);
  }

  if(atexit(removeFifo) != 0){
    printf("atexit" );
    exit(EXIT_FAILURE);
  }

  /*Создаем запрос, открываем серверную очередь FIFO и отпровляем запрос */
  req.pid = getpid();


  if(argc == 2){
     if((req.seqLen = atoi(argv[1])) == 0){
       printf("Eror atoi");
       exit(EXIT_FAILURE);
     }
  }
  else if(argc = 1){
    req.seqLen = 1;
  }

  /*Открываем FIFO на записть который слушает сервер */
  serverFd = open(SERVER_FIFO, O_WRONLY);
  if(serverFd == -1){
    printf("mkfifo %s\n", SERVER_FIFO);
    exit(EXIT_FAILURE);
  }

  /*Передаем серверу запрос структуру со сосвоим pid и дленну последовательности */
  if(write(serverFd, &req, sizeof(struct request)) != sizeof(struct request)){
    printf("Can't write to server");
    exit(EXIT_FAILURE);
  }

  /*Открываем нашу очередь FIFO, считываем и выводим ответ */

  if((clientFd = open(clientFifo, O_RDONLY)) == -1){
    printf("open %s\n", clientFifo);
    exit(EXIT_FAILURE);
  }

  if(read(clientFd, &resp, sizeof(struct response)) != sizeof(struct response)){
    printf("Can't rea response from server\n");
    exit(EXIT_FAILURE);
  }

  if(close(clientFd) == -1)
    printf("close");
   
  printf("%d\n", resp.seqNum);
  return 0;
}
