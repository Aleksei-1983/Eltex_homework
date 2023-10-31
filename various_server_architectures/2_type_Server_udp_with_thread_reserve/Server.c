/* сервер с резервированием потоков и перераспределение нагруски через именованный калал FIFO*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/stat.h>


#define SERVER_FIFO "/tmp/seqnum_sv" /*имя серверной очереди */
#define SER_PORT 8888
#define NUMBER_THREAD 10
#define THREAD_ITERATIONS 1
#define SIZ_BUFFER 128
#define SIZ_BUFFER_MK sizeof(struct sockaddr_in) + SIZ_BUFFER
#define SERVER_ADDR "127.0.0.1"

void signal_handler(int signum){

  if(unlink(SERVER_FIFO) == -1){
    perror("unlink: ");
    exit(EXIT_FAILURE);
  }

  printf("\nServer end. \n");
  exit(1);
}

void* thread_function(){

  int index;
  int port;
  char bufferThreadMk[SIZ_BUFFER_MK];
  char bufferSend[SIZ_BUFFER];
  char* bufferMsg;
  struct sockaddr_in* client_addr;
  int queueMkFd;

  int serverFd;
  
  if((queueMkFd = open(SERVER_FIFO, O_RDONLY)) == -1){
    perror("open_R:");
    exit(EXIT_FAILURE);
  }

  // создаем сокит
  serverFd = socket(AF_INET, SOCK_DGRAM, 0);
  if (serverFd == -1) {
    perror("socket thread:");
    exit(EXIT_FAILURE);
  }

  for(index = 0; index < THREAD_ITERATIONS; index++){
    
    memset(bufferThreadMk, 0, SIZ_BUFFER_MK);
    // Ждем в очереди сообщений адрес и порт клиента
    if(read(queueMkFd, bufferThreadMk, SIZ_BUFFER_MK) != SIZ_BUFFER_MK) {
      printf("Error read tid: %ld", pthread_self());
    }
  
    client_addr = (struct sockaddr_in*)bufferThreadMk;
    bufferMsg = bufferThreadMk + sizeof(struct sockaddr_in);
    printf("thread: %ld, port client: %d, msg: %s\n", pthread_self(), ntohs(client_addr->sin_port), bufferMsg);
  
    // отпровляем клиенту по udp
    snprintf(bufferSend, SIZ_BUFFER, "Hi client by potr: %d, i recv msg(%s)", ntohs(client_addr->sin_port), bufferMsg);
    if(sendto(serverFd, bufferSend, strlen(bufferSend), 0, (struct sockaddr*) client_addr, sizeof(*client_addr)) == -1){
      perror("sendto:");
      exit(EXIT_FAILURE);
    }
  }
  if(close(serverFd) == -1){
    perror("close_socket: ");
    exit(EXIT_FAILURE);
  }
  
  if(close(queueMkFd) == -1){
    perror("close_mk: ");
    exit(EXIT_FAILURE);
  }

  pthread_exit(NULL);
}

int main(int argc, char* argv){ 

  int index;
  
  int fileMk;
  int serverMkFd;
  char bufferMk[SIZ_BUFFER_MK];

  int serverFd;
  int clientFd;
  char buffer[SIZ_BUFFER];
  struct sockaddr_in Saddr_1, client_addr, client_addr_1;
  socklen_t client_addr_len = sizeof(client_addr);

  int thread;
  pthread_t threads[NUMBER_THREAD] = {0};

  struct sockaddr_in *ptr_addr  = (struct sockaddr_in*)bufferMk;
  char* ptr_date = bufferMk + sizeof(struct sockaddr_in); 

  // создаем именнованную очередь

  fileMk = mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP );

  if (fileMk == -1 && errno != EEXIST){
    perror("mkfifo:");
    exit(EXIT_FAILURE);
  }

    // создаем пул потоков
  for(index = 0; index < NUMBER_THREAD; index++){
    thread = pthread_create(&threads[index], NULL, thread_function, NULL);
    if(thread != 0 ){
      perror("pthread_create:");
      exit(EXIT_FAILURE);
    }
  }

  /*Открываем дополнительный записывающий дескриптор
    чтобы некогда не получать конец файла*/

  if ((serverMkFd = open(SERVER_FIFO, O_WRONLY)) == -1){
    perror("open_W:");
    exit(EXIT_FAILURE);
  }

  
  // создаем новый серверный сокет udp и биндим его
  serverFd = socket(AF_INET, SOCK_DGRAM, 0);
  if (serverFd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&Saddr_1, 0, sizeof(Saddr_1));
  Saddr_1.sin_family = AF_INET;
  // Saddr_1.sin_addr.s_addr = inet_addr(SERVER_ADDR);
  Saddr_1.sin_addr.s_addr = INADDR_ANY;
  Saddr_1.sin_port = htons(SER_PORT);
  
  if (bind(serverFd, (struct sockaddr *)&Saddr_1, sizeof(Saddr_1)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
    
  for(index = 0; index < (NUMBER_THREAD * THREAD_ITERATIONS); index++){
    // принемает сообщение от клиента
    if(recvfrom(serverFd, buffer, SIZ_BUFFER, 0, (struct sockaddr*) &client_addr, &client_addr_len ) == -1){
      perror("recvfrom:");
      exit(EXIT_FAILURE);
    }

    memset(bufferMk, 0, SIZ_BUFFER_MK);
    memcpy(ptr_addr, &client_addr, client_addr_len);
    memcpy(ptr_date, buffer, SIZ_BUFFER);

   
    if(write(serverMkFd, bufferMk, SIZ_BUFFER_MK) != SIZ_BUFFER_MK ){
      printf("Error writng to FIFO\n");
    }
    
  }

  usleep(10000);
  printf("=============================================================\n");
  for (index = 0; index < NUMBER_THREAD; index++) {
    if (pthread_join(threads[index], NULL) != 0) {
      perror("pthread_join");
    }
    printf("thread clear = %d\n", index);
  }

  if(close(serverMkFd) == -1){
    perror("close: ");
    exit(EXIT_FAILURE);
  }

  if(unlink(SERVER_FIFO) == -1){
    perror("unlink: ");
    exit(EXIT_FAILURE);
  }
}
