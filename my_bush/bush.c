/* Программа имитирует оболочку баш запуская программу в дочернем процессе*/

#include "bush.h"

int main (int argc, char** argv)
{
  int index = 0;
  int index1; 
  int status;
  char buf[SIZE];
  char* pathToFile = "/bin/";
  pid_t pid;
  
  /*Формируем в буфере адрес запуска программы */
  for (; index < strlen(pathToFile); index++){
    buf[index] = pathToFile[index];
  }

  for (index1 = 0; index < (strlen(pathToFile) + strlen(argv[1])); index++, index1++){
    buf[index] = argv[1][index1];
  }
  buf[index] = '\0';
  

  /* Создаем дочерний процес  и запускаем в нем программу
   * адрес которой сформировали в буфере*/
  switch(pid = fork()){
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);
    break;

  case 0:
    execl(buf, argv[1],argv[2], NULL);

  befault:
    wait(&status);
    break;
  }
  return 0;
}
