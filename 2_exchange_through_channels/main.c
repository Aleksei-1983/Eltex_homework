/* Программа отправляет дочернему процессу строку   получает ответ и выводит на экран */
#include "main.h"


int main(int argc, char** argv)
{
  int pipeFd1[2];
  int pipeFd2[2];
  
  int numRead;
  char buf[SIZE];
  
  pid_t pid;
  
  char* outWord = "Hello world!!";
  
  if (pipe(pipeFd1) == -1)
    perror("pipe");

  if (pipe(pipeFd2) == -1)
    perror("pipe");

  switch(pid = fork()){
  case -1:
    perror("fork");
    break;
  case 0:
    char* inWord = "Hello Human!!";
    
    if(close(pipeFd1[1]) == -1 || close(pipeFd2[0]) == -1){
      perror("close - child1");
      exit(EXIT_FAILURE);
    }
    
    if ((numRead = read(pipeFd1[0], buf, SIZE)) == -1){
      perror("read");
      exit(EXIT_FAILURE);
    }
    
    if(strcmp(outWord, buf) == 0){
      if((numRead = write(pipeFd2[1], inWord, strlen(inWord))) == -1){
	perror("write");
      exit(EXIT_FAILURE);
      }
    }

    if(close(pipeFd1[0]) == -1 || close(pipeFd2[1]) == -1){
      perror("close - child2");
      exit(EXIT_FAILURE);
    }
    
    break;

  default:
    
    if(close(pipeFd1[0]) == -1 || close(pipeFd2[1]) == -1){
      perror("close - parent1");
      exit(EXIT_FAILURE);
    }

    if ((numRead = write(pipeFd1[1], outWord, strlen(outWord))) == -1){
      perror("write - parent");
    }
    
    if ((numRead = read(pipeFd2[0], buf, SIZE)) == -1){
      perror("read - parent");
    }

    if(close(pipeFd1[1]) == -1 || close(pipeFd2[0]) == -1){
      perror("close - parent2");
    }

    wait(NULL);
    
    printf("1> %s\n", outWord);
    printf("2> %s\n", buf);
    
    break;
  }
  return 0;
}
