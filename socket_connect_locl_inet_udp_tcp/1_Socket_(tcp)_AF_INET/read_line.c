#include <unistd.h>
#include <errno.h>
#include "read_line.h"


ssize_t readLine(int fd, void* buffer, size_t len){

  ssize_t numRead;
  size_t totRead;
  char* buf;
  char ch;

  if(len <= 0 || buffer == NULL){
    

    /*EINVAL: Этот код ошибки указывает на то, что системному вызову был передан недопустимый аргумент.
     *Это может произойти, например, если процесс пытается открыть файл, который не существует,
     *или если он пытается установить недопустимую опцию для сокета.
     *Процесс может затем проверить значение errno, чтобы узнать,
     *была ли ошибка вызвана недопустимым аргументом, и принять соответствующие меры*/
    errno = EINVAL;
    return -1;
  }

  buf = buffer;
  totRead = 0;

  while(1){
    numRead = recv(fd, &ch, 1, 0);
    if(numRead == -1){
     /*EINTR: Этот код ошибки указывает на то,
     *что системный вызов был прерван сигналом до его завершения.
     *Это может произойти, например, если процесс ожидает ввода и получает сигнал,
     *который прерывает ожидание. Процесс может затем проверить значение errno,
     *чтобы узнать, была ли ошибка вызвана сигналом, и принять соответствующие меры*/
      if(errno == EINTR)
	continue;
      else
	return -1;
    }
    else if(numRead == 0){
      if(totRead == 0)
	return 0;
      else
	break;
    }
    else{
      if(totRead < len -1){
	totRead++;
	*buf = ch;
	buf++;
      }

      if(ch == '\n')
	break;
    }
  }

  *buf = '\0';
  return totRead;
}
