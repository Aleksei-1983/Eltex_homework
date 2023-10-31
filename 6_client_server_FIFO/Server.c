#include "pipes_name_fifo.h"
#include <signal.h>

void signal_handler(int signum)
{
  int fileUnlink;
  
  fileUnlink = unlink(SERVER_FIFO);

  if (fileUnlink == -1){
    printf("unlink %s\n", SERVER_FIFO);
    exit(EXIT_FAILURE);
  }

  printf("\nServer end\n");
  exit(0);
}

int main(int argc, char** argv)
{
  
  int fileMk;
  int serverFd;
  int dummyFd;
  int clientFd;
  char clientFifo[CLIENT_FIFO_NAME_LEN];
  struct request req;
  struct response resp;
  int seqNum = 0;

  signal(SIGINT,signal_handler);
  
  /*Создаем очередь и открываем ее для чтения*/
  umask(0); /*получаем нужные нам прова*/
  
  fileMk = mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP);

  if (fileMk == -1 && errno != EEXIST){
    printf("mkfifo %s\n", SERVER_FIFO);
    exit(EXIT_FAILURE);
  }

  serverFd = open(SERVER_FIFO, O_RDONLY);
  if(serverFd == -1 ){

    switch(errno){
      case ENOENT:
	printf("File %s does not exit\n", SERVER_FIFO);
        break;
	
      case EACCES:
	printf("No file %s\n permissions", SERVER_FIFO );
        break;
	
      default:
	printf("File %s eccess error\n", SERVER_FIFO);
	break;
    }
    exit(EXIT_FAILURE);
  }
  
  /*Открываем дополнительный записывающий дескриптор
   чтобы некогда не получать конец файла*/

  if ((dummyFd = open(SERVER_FIFO, O_WRONLY)) == -1){
    printf("open %s\n", SERVER_FIFO);
    exit(EXIT_FAILURE);
  }

  /*считываем запросы к серверу*/

  for(;;){
    if(read(serverFd, &req , sizeof(struct request)) != sizeof(struct request)){
      printf("Error reading request\n");
      continue;
      
    }
    /*Открываем клиенскую очередь FIFO (предворительно созданную клиентом) */

    snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, (long) req.pid);

    if((clientFd = open(clientFifo, O_WRONLY)) == -1){
      printf("open %s\n", clientFifo);
      continue;
    }

    /*Отпровляем ответ клиенту и закрываем очередь FIFO*/

    resp.seqNum = seqNum;
    if(write(clientFd, &resp, sizeof(struct response)) != sizeof(struct response)){
      printf("Error writng to FIFO %s \n", clientFifo);
    }
    printf("Client_pid == %d\n",(int)req.pid);
    
    if(close(clientFd) == -1)
      printf("close");
  
    /* Обновляем номер нашей последовательности*/
    seqNum += req.seqLen;

  }
  return 0;
}
