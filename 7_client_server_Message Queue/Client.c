#include "Server_Client.h"


int main(int argc, char** argv)
{
  char ch;
  int index = 0;
  int numberRequests;
  int msgPid;
  key_t key;
  ssize_t numberBytes;
  char* str = "Client -> Server";
  struct msgBuf msg;

  numberRequests = 3;

  key = ftok("Server.c", 1);
  
  msgPid = msgget(key, 0);
  if(msgPid == -1){
    perror("msgget");
    exit(EXIT_FAILURE);
  }


  for(;;){
    /* Отпровляется на сервер указоное в оргументе количество запросов */
    
    for(; index < numberRequests; index++){

      msg.mtype = 1;
      msg.date.pid = getpid();
      requestStr(msg.date.mtext, str, index);
      
      if(msgsnd(msgPid, &msg, sizeof(msg.date), 0 ) == -1){
      perror("msgsnd");
      exit(EXIT_FAILURE);
      }
    }

    /* Проверяем приоритет со номеров своео пида и выводим ответ сервера*/
    if((numberBytes = msgrcv(msgPid, &msg, sizeof(msg.date), getpid(), 0)) == -1){
      perror("msgrcv");
      exit(EXIT_FAILURE);
    }
    else if(numberBytes > 0){
      printf("%s\npid = %d\n", msg.date.mtext, (int)msg.date.pid);
    }
    
  }

  return 0;

}
