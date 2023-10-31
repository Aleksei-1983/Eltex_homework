#include "Server_Client.h"

int main (int argc, char** argv)
{
  char ch;
  int index = 0;
  int msgPid;
  key_t key;
  ssize_t numberBytes;
  char* str = "Server -> Client";
  struct msgBuf msg;


  key = ftok("Server.c", 1 );
  /* Создаем очередь сообщений с провами записи и чтения
   * только для пользователя */
  msgPid = msgget(key, 0600 | IPC_CREAT);
  if (msgPid == -1){
    perror ("msgget");
    exit(EXIT_FAILURE);
  }

  for(;;){

    /* Слушает в канале свой приаретет если получил сообшение
     * от клиента то формирует и отпровляет сообщение с приоритетом
     * пида клиента*/
    if((numberBytes = msgrcv(msgPid, &msg, sizeof(msg.date), 1,0)) > 0){
      
      printf("%s\npid = %d\n", msg.date.mtext, (int)msg.date.pid);
      msg.mtype = (long)msg.date.pid;
      msg.date.pid = getpid();
      requestStr(msg.date.mtext, str, index);
      index++;
      
      printf("%s\npid = %d\n", msg.date.mtext, (int)msg.date.pid);

      /* Отпровляем сообщение в очередь сообщений*/
      if(msgsnd(msgPid, &msg, sizeof(msg.date), 0) == -1){
	perror("msgsnd");
	exit(EXIT_FAILURE);
      }  
    }
    else if(numberBytes == -1){
      perror("msgrcv");
      exit(EXIT_FAILURE);
  
    }
    
    /* Удоляем очередь сообщений */
    if(index == 20){
      if(msgctl(msgPid, IPC_RMID, NULL) == -1){
      perror("msgctl");
      exit(EXIT_FAILURE);
      }

    }
    
  }
  
  return 0;
    
}
