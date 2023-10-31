#ifndef READ_LINE_H
#define READ_LINE_H

#include <sys/types.h>
#include <sys/socket.h>
/* Функция читает из дискрипрора файла (подклученного сокита )
 * строку до конца строки и помещает ее в переданный буфер с указанием  * размера буфера.
 * fd -> дескриптор 
 * buffer-> буфер
 * len -> размер буфера в байтах
 * return -> если небыло ошибок возвращает количества байт в буфере */
ssize_t readLine(int fd, void* buffer, size_t len);

#endif 
