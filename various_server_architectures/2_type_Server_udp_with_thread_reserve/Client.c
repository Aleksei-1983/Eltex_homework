/* слиент создает заданное число потоков и количество итераций в каждом потоке
 * и они начинают все отпровлять серверу запросы и ждать ответы
 * потом выводять полученный ответ и время ожидания на экран */
#include <signal.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>


#define _BSD_SOURCE

#define BACKLOG 5

#define CLIENT_PORT "8888"
#define SERVER_PORT 8888
#define SERVER_ADDR "127.0.0.1"
#define MAX_THREADS 10
#define N 1
#define SIZ_BUFFER 128

void *thread_function(void *arg) {
  int thread_number = *(int*)(arg);
  int sockFd;
  
  struct sockaddr_in server_address;
  socklen_t server_address_len = sizeof(server_address);
  
  char buffer[SIZ_BUFFER];

  struct timeval start_time, end_time;
  double waiting_time;

  sockFd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sockFd == -1){
    perror("socket_thread:");
    exit(EXIT_FAILURE);
  }

  
  for(int j = 0; j < N; j++ ){
    
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    
    memset(buffer, 0, SIZ_BUFFER);
    snprintf(buffer, SIZ_BUFFER, "Hi i client: %d", thread_number);
    // Measure waiting time in milliseconds
    gettimeofday(&start_time, NULL);

    sendto(sockFd, (const char*)buffer, strlen(buffer), 0, (const struct sockaddr*)&server_address, sizeof(server_address));
        
    int n = recvfrom(sockFd, (char*)buffer, SIZ_BUFFER, MSG_WAITALL, (struct sockaddr*)&server_address, &server_address_len);

    buffer[n] = '\0';

    gettimeofday(&end_time, NULL);
    waiting_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0;
    waiting_time += (end_time.tv_usec - start_time.tv_usec) / 1000.0;

    // Compare sent and received numbers
  
    printf("received %s; thread_number = %d; from server port = %d; time  %.2f ms\n",buffer,thread_number, ntohs(server_address.sin_port) , waiting_time);

  }

  usleep(1000); // засыпаем на 100 миллисекунд
  //printf("Connect to server on %d port\n", server_port);
  close(sockFd);
  pthread_exit(NULL);
}

int main() {
  
  pthread_t threads[MAX_THREADS];
  int thread_args[MAX_THREADS];
  int i;

  // Initialize thread array
  memset(threads, 0, sizeof(threads));

  // Create threads
  for (i = 0; i < MAX_THREADS; i++) {
    thread_args[i] = i;
    if(pthread_create(&threads[i], NULL, thread_function, &thread_args[i]) != 0){
      printf("pthread_create thread = %d error", i); 
      exit(EXIT_FAILURE); 
    }
    usleep(1); // засыпаем 
  }

  usleep(10000);
  printf("=============================================================\n");
  // Wait for threads to finish
  for (i = 0; i < MAX_THREADS; i++) {
    if (threads[i] != 0) {
      pthread_join(threads[i], NULL);
    }
  }

  return 0;
  }

