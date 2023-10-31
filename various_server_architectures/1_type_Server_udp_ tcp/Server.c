/* сервер ждет подключение от клиента, когда подключение произошло 
 * создает поток и отпровляет новый порт клиент получает этот порт и 
 * перепотключается к нему и обменивается с сервером сообщением  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <termios.h>
#include <fcntl.h>


#define MAX_THREADS 10000
#define SER_PORT 7777
#define SERVER_ADDR "127.0.0.1"
#define BACKLOG 5

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

struct thread_args {
  char* _clientIp;
  int _port;
  int _serverFd;
  int _thread_number;
};

int threadsMas[MAX_THREADS] = {0};
//int semid; // Идентификатор массива семафоров




void *thread_function(void *args) {

  struct thread_args* threadArg = (struct thread_args*) args; 

  // printf("Cfd = %d; threadnumber = %d; pid = %ld;\n",  Cfd,threadIndex, pthread_self());
  int serverFd, clientFd;
  char buffer[1024];
  int index;
  struct sockaddr_in Saddr_1, client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  socklen_t Saddr_1_len = sizeof(Saddr_1);

  // создаем новый серверный сокет
  serverFd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverFd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  // биндем его
  memset(&Saddr_1, 0, sizeof(Saddr_1));
  Saddr_1.sin_family = AF_INET;
  Saddr_1.sin_addr.s_addr = htonl(INADDR_ANY);
  Saddr_1.sin_port = 0;
  if (bind(serverFd, (struct sockaddr *)&Saddr_1, sizeof(Saddr_1)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  // слушаем
  if (listen(serverFd, BACKLOG) == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  // получаем порт нового сокета
  if (getsockname(serverFd, (struct sockaddr *)&Saddr_1, &Saddr_1_len) == -1) {
    perror("getsockname");
    exit(EXIT_FAILURE);
  }

  // Convert the port number to a string and print it on the screen
  sprintf(buffer, "%d", ntohs(Saddr_1.sin_port));
  //printf("Cfd = %d\n", Cfd);
  // printf("port = %s\n", buffer);

  
  memset(&client_addr, 0, sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_addr.s_addr = inet_addr(threadArg->_clientIp);
  client_addr.sin_port = htons(threadArg->_port);
  // отпровляем новый порт клиенту
  sendto(threadArg->_serverFd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, client_addr_len);
  
 

  // Cfd_1 accept на новый сокет
  clientFd = accept(serverFd, (struct sockaddr *)&Saddr_1, &Saddr_1_len);
  if (clientFd == -1) {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  // как подключились
  printf("Connected to client to port = %s; thread = %d\n", buffer, threadArg->_thread_number);

  // читаем данные
  if (recv(clientFd, buffer, sizeof(buffer), 0) == -1) {
    perror("recv");
    exit(EXIT_FAILURE);
  }

  // делаем работу с данными
  for(index = 0; buffer[index] != '\0'; index++){
    buffer[index] = toupper(buffer[index]);
  }
    
  //printf("Received message: %s\n", buffer);

  // отпровляем данные
  if (send(clientFd, buffer, strlen(buffer), 0) == -1) {
    perror("send_Cfd_1");
    exit(EXIT_FAILURE);
  }
  
  usleep(1000); // засыпаем на 100 миллисекунд


  // закрываем Cfd_1
  close(clientFd);
  // закрываем сокет
  close(serverFd);

  pthread_exit(NULL);
}



int main() {


  int serverFd, clientFd;
  struct sockaddr_in serverAddr, clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);
  pthread_t threads[MAX_THREADS] = {0};
  int i;
  union semun arg;
  struct thread_args threadArg;
  int thread;
  char buffer[2];
  int opt = 1;

  int flags = fcntl(STDIN_FILENO, F_GETFL, 0); // получение текущих флагов
  fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK); // установка неблокирующего режима
  
  char ch;

  // создает сокит tcp инициализируем его нулями
  serverFd = socket(AF_INET, SOCK_DGRAM, 0);
  if (serverFd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  
  memset(&serverAddr, 0, sizeof(serverAddr));
  // биндит его c с портос 7777
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
  // serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(SER_PORT);
    
  if (bind(serverFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  i = 0;
  // в цикле
  while (i < MAX_THREADS) {
    //read(STDIN_FILENO, &ch, 1); // считывание символа

    if(recvfrom(serverFd, buffer, 2, 0, (struct sockaddr*) &clientAddr, &clientAddrLen) > 0){
      threadArg._port = ntohs(clientAddr.sin_port);
      threadArg._clientIp = inet_ntoa(clientAddr.sin_addr);
      threadArg._serverFd = serverFd;
      threadArg._thread_number = i;
    }

    // создает поток ( масив пидов[], передает функцию thread_function , и Cfd)
    thread = pthread_create(&threads[i], NULL, thread_function, (void*)&threadArg);
    if (thread == 0) {
      threadsMas[i] = 1;
      //printf("clientfd = %d; threadnumber = %d; thread_pid = %ld\n ", threadArg._clientFd, threadArg._threadNumber, threads[i]);
      usleep(10); // засыпаем на 100 миллисекунд
      i++;   
    }

  }
  printf("=============================================================\n");
  for (i = 0; i < MAX_THREADS; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      perror("pthread_join");
    }
    printf("thread clear = %d\n", i);
  }

  close(serverFd);

  return 0;
}

