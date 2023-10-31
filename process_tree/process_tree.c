/* Программа порождает дерево дочерних процесов*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main (char* argc, char** argv)
{
  pid_t pid;
  int wstatus1;
  int wstatus2;
  
  pid = fork();
  
  if(0 == pid){
    pid = fork();
    if (0 == pid){
      printf("3 pid = %d, ppid = %d\n", getpid(), getppid());
    }
    else if(0 < pid){
      wait(&wstatus1);
      pid = fork();
      if(0 == pid){
	printf("3 pid = %d, ppid = %d\n", getpid(), getppid());
      }
      else if(0 < pid){
	wait(&wstatus2);
	printf("2 pid = %d, ppid = %d\n", getpid(), getppid());
      }
    }
  }
  else if (0 < pid){
    wait(&wstatus1);
    printf("1 pid = %d, ppid = %d\n", getpid(), getppid());
    pid = fork();
    if(0 == pid){
      printf("2 pid = %d, ppid = %d\n", getpid(), getppid());
      pid = fork();
      if(0 == pid){
	printf("3 pid = %d, ppid = %d\n", getpid(), getppid());
      }
      else if(0 < pid){
	wait(&wstatus1);
      }
    }
    else if(0 < pid ){
      wait(&wstatus1);
      
    }
  }

  return 0;
}
