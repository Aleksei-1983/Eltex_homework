#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
  int pipefd[2];
  int pipefd1[2];
  char buff;
  pid_t pid;

  if (argc != 2){
    exit(EXIT_FAILURE);
  }

  if (pipe(pipefd) == -1){
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  printf("pipe 1 ; 0 = %d, 1 = %d\n", pipefd[0], pipefd[1]);

  if (pipe(pipefd1) == -1){
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  printf("pipe 2 ; 0 = %d, 1 = %d\n", pipefd1[0], pipefd1[1]);
  
  pid = fork();

  if(0 == pid){
    close(pipefd[1]);
    while(0 < read(pipefd[0], &buff, 1))
      write(STDOUT_FILENO, &buff, 1);

    write(STDOUT_FILENO, "\n", 1);
    close(pipefd[0]);
  }
  else if(0 < pid){
    close(pipefd[0]);

    write(pipefd[1], argv[1], strlen(argv[1]));
    close(pipefd[1]);
    wait(NULL);
    
  }
  else{
    perror("fork");
    exit(EXIT_FAILURE);
  }
  
  return 0;
}
