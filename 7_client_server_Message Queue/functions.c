#include "Server_Client.h"

/* Функция преобразует число в строку и формирует сообщение
 * строка входная
 * строка входная
 * индекс выходной строки*/
void requestStr(char* reqest, char* str, int index)
{
  int strln = strlen(str);
  char bufStr[SIZE_STR];

  snprintf(bufStr, sizeof(bufStr), "%d", index);
  
  if ((strlen(bufStr) + strln) < MAX_MSG_SIZE){
    sprintf(reqest, "%s. %s", bufStr, str);
  }
  else{
    reqest[0] = '\0';
  }
}
