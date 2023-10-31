/* Сервер спомощью select() отслеживает дескрипторы и если от клиента что нибуть пришло 
 * на tcp или udp то отвечает ему */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <netinet/in.h>

#define PORT 8888
#define SIZ_BUF 128
#define MAX(x, y) ((x) >= (y)) ? (x) : (y) 

int main(int argc, char const *argv[]) {
  fd_set masterFds;
  fd_set readFds;
  int maxFd = 0;

  int listenerTcpFd;
  int listenerUdpFd;
  int newTcpFd;

  struct sockaddr_in serverAddr;
  struct sockaddr_in clientAddr;

  struct timeval tv;
  int resultSelect;

  socklen_t addrlen = sizeof(struct sockaddr_in);

  int nbyts;
  int index;
  int indexSoClTcp;
  char buf[SIZ_BUF];
  char addr[INET_ADDRSTRLEN];

  FD_ZERO(&masterFds);
  FD_ZERO(&readFds);

  if((listenerTcpFd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    perror("socket tcp");
    exit(EXIT_FAILURE);
  }
  
  FD_SET(listenerTcpFd, &masterFds);
  maxFd = MAX(maxFd, listenerTcpFd);
  
  if((listenerUdpFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
    perror("socket udp");
    exit(EXIT_FAILURE);
  }

  FD_SET(listenerUdpFd, &masterFds);
  maxFd = MAX(maxFd, listenerUdpFd);
  
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(PORT);

  if(bind(listenerTcpFd, (struct sockaddr*)&serverAddr, addrlen) == -1){
    perror("bind tcp");
    exit(EXIT_FAILURE);
  }

  if(bind(listenerUdpFd, (struct sockaddr*)&serverAddr, addrlen) == -1){
    perror("bind udp");
    exit(EXIT_FAILURE);
  }

  if(listen(listenerTcpFd, 5)){
    perror("listen");
    exit(EXIT_FAILURE);
  }
  
  for(;;){
    readFds = masterFds;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    if((resultSelect =  select(maxFd + 1, &readFds, NULL, NULL, &tv )) == -1){
      perror("select");
      exit(EXIT_FAILURE);
    }

    for(index = 0; (index <= maxFd) && (resultSelect != 0); index++){
      if(FD_ISSET(index, &readFds)){
	if(index == listenerTcpFd){
	  
	  /* Обрабатываем подключения по tcp*/
	  if((newTcpFd = accept(listenerTcpFd, (struct sockaddr*)&clientAddr, &addrlen)) == -1){
	    perror("accept");
	  }else{
	    FD_SET(newTcpFd, &masterFds);
	    maxFd = MAX(maxFd, newTcpFd);
	    inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, addr, INET_ADDRSTRLEN);
	    printf("new connection from %s on socket %d\n",addr, newTcpFd);
	  }
	  
	}else if(index == listenerUdpFd){
	  memset(buf, 0, SIZ_BUF);
	  if((nbyts = recvfrom(listenerUdpFd, buf, SIZ_BUF, 0, (struct sockaddr*)&clientAddr, &addrlen)) == -1){
	    perror("recvfrom");
	  }else{
	    buf[nbyts] = '\0';
	    printf("%s\n", buf);
	    strcpy(buf, "Hi client udp i server!");
	    if(sendto(listenerUdpFd, buf, SIZ_BUF, 0, (struct sockaddr*)&clientAddr, addrlen) == -1){
	      perror("sendto");
	    }
	  }
	  
	}else{
	  memset(buf, 0, SIZ_BUF);
	  if((nbyts = recv(index, buf, SIZ_BUF, 0)) <= 0){
	    if(nbyts == 0){
	      printf("selectserver: client close connection on socket %d\n", index);
	    }else{
	      perror("recv");
	    }
	    
	    close(index);
	    FD_CLR(index, &masterFds);
	    
	  }else{
	    printf("%s\n", buf);
	    strcpy(buf, "Hi client tcp, i setrer!");
	    if(send(index, buf, SIZ_BUF, 0) == -1){
	      perror("send:");
	    }
	      	    
	  }
	}
	
      }
    }
    
  }
  return 0;
}


