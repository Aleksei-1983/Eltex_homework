/* пример с работы функции select()*/
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define STDIN 0
#define SIZ_BUF 128

int main(int argc, char* argv){
  struct timeval tv;
  fd_set  mainFds;
  fd_set readFds;
  int result_select;
  int read_bytt;
  char buffer[SIZ_BUF];
  /*  выстовления время ожидания */
  

  FD_ZERO(&mainFds);
  FD_SET(STDIN, &mainFds);

  for(;;){
    readFds = mainFds;
    /* при каждом вызове селект структура времени обнуляется
     * и надо заполнить время ожидания заново*/
    tv.tv_sec = 3;
    tv.tv_usec = 500000;
  
    if(-1 ==  (result_select = select(STDIN + 1, &readFds, NULL, NULL, &tv))){
      perror("select: ");
      exit(EXIT_FAILURE);
    }else if(0 == result_select){
      printf("Timeout occurred\n");
    }else{
      if(FD_ISSET(STDIN, &readFds)){
	if( -1 == (read_bytt = read(STDIN, buffer, SIZ_BUF))){
	  perror("read: ");
	  exit(EXIT_FAILURE);
	}else{
	  buffer[read_bytt] = '\0';
	  printf("> %s\n", buffer);
	}
      }
    }    
  }

  return 0;
}
