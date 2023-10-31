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
#define SERVER_PORT 7777
#define SERVER_ADDR "127.0.0.1"
#define MAX_THREADS 100
#define N 1
#define SIZ_BUFFER 128

void *thread_function(void *arg) {
  int thread_number = *(int*)(arg);
  int client_socket, client_socket2;
  struct sockaddr_in server_address, client_address ;
  socklen_t len;
  char buffer[SIZ_BUFFER];

  int server_port;
  struct timeval start_time, end_time;
  double waiting_time;

  
  for(int j = 0; j < N; j++ ){
    // Measure waiting time in milliseconds
    gettimeofday(&start_time, NULL);
    // Create TCP socket
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      perror("socket creation failed");
      exit(EXIT_FAILURE);
    }
 
    // Connect to local host with port number 7777
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
  
    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_ADDR, &server_address.sin_addr) <= 0) {
      printf("\nInvalid address/ Address not supported \n");
      exit(EXIT_FAILURE);
    }
    if(bind(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) != -1){
      perror("bind");
      exit(EXIT_FAILURE);
    }

    
    memset(buffer, 0, SIZ_BUFFER);
    buffer[0] = '.';
    buffer[1] = '\0';
    sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&server_address, sizeof(server_address));

    if(recvfrom(client_socket, buffer, SIZ_BUFFER, 0, (struct sockaddr*)&client_address, &len ) == -1){
      perror("recvfrom");
      exit(EXIT_FAILURE);
    }
    
    //printf("Connect to server on %s port\n", buffer);
    server_port = atoi(buffer);
    //printf("thread_number = %d; new port = %d\n",thread_number, server_port);
    // Create TCP socket number 2
    if ((client_socket2 = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      perror("socket creation failed");
      exit(EXIT_FAILURE);
    }
  
    // Reconnect to local host with the received port number
    // memset(&server_address, 0, sizeof(server_address));
    //server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    //server_address.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    if(bind(client_socket2, (struct sockaddr*)&server_address, sizeof(server_address)) != -1){
      perror("bind2");
      exit(EXIT_FAILURE);
    }

    // Send string
    strcpy(buffer, "client");
    sendto(client_socket2, buffer, strlen(buffer), 0, (struct sockaddr*)&server_address, sizeof(server_address));

    memset(buffer, 0, sizeof(buffer));
    if(recvfrom(client_socket2, buffer, SIZ_BUFFER, 0, (struct sockaddr*)&client_address, &len ) == -1){
      perror("recvfrom");
      exit(EXIT_FAILURE);
    }

    gettimeofday(&end_time, NULL);
    waiting_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0;
    waiting_time += (end_time.tv_usec - start_time.tv_usec) / 1000.0;

    // Compare sent and received numbers
  
    printf("received %s; thread_number = %d; from server port = %d; time  %.2f ms\n",buffer,thread_number, server_port , waiting_time);

    usleep(100000); // засыпаем на 100 миллисекунд
    //printf("Connect to server on %d port\n", server_port);
    close(client_socket);
    // Close connection
    close(client_socket2);
  }
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
    usleep(500); // засыпаем 
  }
  printf("=============================================================\n");
  // Wait for threads to finish
  for (i = 0; i < MAX_THREADS; i++) {
    if (threads[i] != 0) {
      pthread_join(threads[i], NULL);
    }
  }

  return 0;
  }

