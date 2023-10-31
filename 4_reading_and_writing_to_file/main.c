/* Программа создает файл записывает в него строку 
и закрывает потом открывает его вновь и читает его 
и выводит на экран, удаляет файл */

#include "main.h"


int main (int argc, char argv)
{
  int fd;
  char* filename =  "exampl.txt";
  char* str = "Hello, world!!";
  char buffer[SIZE];

  if((fd = open(filename, O_WRONLY | O_CREAT, 0400)) == -1){
    printf("Error opening file\n");
    return 1;
  }
    
  write(fd, str , strlen(str));
  close(fd);

  if((fd = open(filename, O_RDONLY)) == -1){
    printf("Error opening file\n");
    return 1;
  }

  read(fd, buffer, SIZE);
  printf("%s", buffer);
  printf("\n");
  
  close(fd);

  if(unlink(filename) == -1){
    printf("Error deleting file\n");
    return 1;
  }

  
  return 0;
}
