// client.c
#include "Server_Client.h"

int main(int argc, char** argv) {

  char reqStr[SIZ_STR];
  char ansStr[SIZ_STR + ADDRSTRLEN];

  char sendStr[SIZ_STR];
  char sendaddr[NI_MAXSERV];
  
  int cfd;
  ssize_t numRead;
  struct addrinfo hints;
  struct addrinfo* result;
  struct addrinfo* rp;

  if(argc == 3){
    strncpy(sendStr, argv[2], SIZ_STR);
    strncpy(sendaddr, argv[1], NI_MAXSERV);
  }
  else{
    strncpy(sendStr, "Hi i client", SIZ_STR);
    strncpy(sendaddr, "127.0.0.1", NI_MAXSERV);
  }

  /* Вызываем getaddrinfo(), чтобы получить список адресов,
     к которым можем попытаться привязать наш сокет */
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_flags      = AI_PASSIVE | AI_NUMERICSERV;
  hints.ai_family     = AF_UNSPEC;
  hints.ai_socktype   = SOCK_STREAM;
  hints.ai_canonname  = NULL;
  hints.ai_addr       = NULL;
  hints.ai_next       = NULL;

  /* для того чтобы выделялся рандомный порт для подключения предаем в функцию второй
   * параметр "0"*/
  if(getaddrinfo(sendaddr,PORT_NUM , &hints, &result) != 0){
    perror("getaddrinfo");
    exit(EXIT_FAILURE);
  }

  for(rp = result; rp != NULL; rp->ai_next){
    cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if(cfd == -1){
      continue; // продолжаем переберать адреса
    }

    if(connect(cfd, rp->ai_addr, rp->ai_addrlen) != -1){
      break;
    }

    close(cfd);
    
  }

  if(rp == NULL){
      printf("Could not connect socket to any address");
      exit(EXIT_FAILURE);
    }
  freeaddrinfo(result);

  if(send(cfd, sendStr, strlen(sendStr), 0) != strlen(sendStr)){
    printf("Error on send\n");
  }

  numRead = recv(cfd, reqStr, SIZ_STR, 0);
 
  if(numRead == -1){
    perror("readline");
    exit(EXIT_FAILURE);
  }
  else if(numRead == 0){
    printf("EOF from server");
    exit(EXIT_FAILURE);
  }
 
  printf("%s\n", reqStr);

  return 0;
}
