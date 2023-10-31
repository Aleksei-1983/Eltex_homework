#include "Server_Client.h"

int main(int argc, char** argv) {

  char reqStr[SIZ_STR];
  char seqStr[SIZ_STR + NI_MAXSERV + 10 ];
  struct sockaddr_storage client;
  int lfd;
  int cfd;
 
  socklen_t addrlen;
  struct addrinfo hints;
  struct addrinfo* result;
  struct addrinfo* rp;
  char port[NI_MAXSERV];

  char addrStrServer[ADDRSTRLEN];
  char hostServer[NI_MAXHOST];
  char serviceServer[NI_MAXSERV];

  char addrStrClient[ADDRSTRLEN];
  char hostClient[NI_MAXHOST];
  char serviceClient[NI_MAXSERV];

  if(signal(SIGPIPE, SIG_IGN) == SIG_ERR){
    exit(1);
  }
  /* Вызываем getaddrinfo(), чтобы получить список адресов,
     к которым можем попытаться привязать наш сокет */
  memset(&hints, 0, sizeof(hints));
  hints.ai_flags      = AI_PASSIVE;
  hints.ai_family     = AF_INET; // выбераем семейчтво подключения 
  hints.ai_socktype   = SOCK_DGRAM;


  // Выделяем порт автоматически
  snprintf(port, sizeof(port), "%d", 0);

  if(getaddrinfo(NULL, PORT_NUM, &hints, &result) != 0){
    perror("getaddrinfo");
    exit(EXIT_FAILURE);
  }

  /* Перебираем полученный список, пока не находим структуру с адресом,
     подходящую для создания и привязывания сокета */
  for(rp = result; rp != NULL; rp = rp->ai_next ){
   
    lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if(lfd == -1){
      //perror("socket");
      printf("%d\n", errno);
      continue;
    }

    
    if(bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0)
      break;

    /* Вызов bind() завершился неудачно; закрываем этот сокет и пробуем следующий адрес */
    close(lfd);
  }

  /* Переводим свой адрес в читаемый вид*/
  if(getnameinfo((struct sockaddr*) rp->ai_addr, rp->ai_addrlen,
		   hostServer, NI_MAXHOST, serviceServer, NI_MAXSERV, 0) == 0){
    snprintf(addrStrServer, ADDRSTRLEN, "(%s, %s)", hostServer, serviceServer);
  }

  if(rp == NULL){
    printf("Could not bind socket to any address");
      perror("listen");
      exit(EXIT_FAILURE);
  }

  if(listen(lfd, BACKLOG) == -1){
    perror("listen");
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(result);

  while(1){
    /* Принимаем соединение со стороны клиента и получаем его адрес */
    addrlen = sizeof(struct sockaddr_storage);
    cfd = accept(lfd, (struct sockaddr*) &client, &addrlen);
    if(cfd == -1){
      perror("accept");
      continue;
    }

    /* Переводим адрес подключенного клиента в читаемый вид*/
    if(getnameinfo((struct sockaddr*) &client, addrlen,
		   hostClient, NI_MAXHOST, serviceClient, NI_MAXSERV, 0) == 0){
      snprintf(addrStrClient, ADDRSTRLEN, "(%s, %s)", hostClient, serviceClient);
    }
    else{
      snprintf(addrStrClient, ADDRSTRLEN,"(?_________?)");
    }

   
    printf("Connection from %s\n", addrStrClient);

    /* Считываем запрос клиента */
    /*
    if(readLine(cfd, reqStr, SIZ_STR) <= 0){
      close(cfd);
      continue;
      }*/

 
    recv(cfd, reqStr, SIZ_STR, 0 );
    snprintf(seqStr, SIZ_STR + NI_MAXSERV + 10, "(%s, port = %s)", reqStr, serviceClient);

     
    /* Отпровляем назад серверу приветствие и его адрес*/
    if(send(cfd, seqStr, strlen(seqStr), 0) != strlen(seqStr)){
      printf("Error on send\n");
    }
    
     printf("Server addr = %s, port = %s send ot Client addr = %s, port = %s\n", hostServer, serviceServer, hostClient, serviceClient );
    
    /* Закрываем соединения*/
    if(close(cfd) == -1){
      perror("close");
    }
  }
  
    return 0;
}
