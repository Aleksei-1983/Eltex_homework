#ifndef __CLIENT__
#define __CLIENT__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <dirent.h>
#include <curses.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SERVER_FIFO "/tmp/seqnum_sv" /*имя серверной очереди */
#define MAX_CLIENTS 10
#define MAX_MSG_LEN 100
#define SIZE_HISTORY 10
#define NUM_ELEMENT_FORM 4


enum tipElementForm {MESSAGE_HISTORY = 0,  MESSAGE_SET, CONTACT_LIST, SERVICE_MESSAGE};

// Структура для хранения информации о клиентах
struct clientInfo {
  int _clientId;
  char _clientName[MAX_MSG_LEN];
  int _connected;
} ;
// список контактов 
struct listClientInfo {
  struct clientInfo* _clientInfo;
  struct listClientInfo* _nexListClientInfo;
};

// Структура для хранения информации о сообщении
struct message{
    int _senderId;
    int _receiverId;
    char _message[MAX_MSG_LEN];
};

struct listMessages{
  struct message* _message;
  struct listMessages* _nexListMessages;
};

//структура запрос серверу на отправку сообщение
struct address {
   int _senderId;
   int _receiverId;
};

//структура окна 
struct formElement
{
  enum tipElementForm _tip;
  WINDOW* _wnd;
  //заполняется если tip CONTACT_LIST
  struct listClientInfo* _listClient;
  int _numClient;
  int _curOnClient;
  // заполняется если tip MESSAGE_SET
  char* _message;
  // заполняется если tip MESSAGE_HISTORY
  struct listMessages* _listMessagesHistory;
  // заполняется если tip SERVICE_MESSAGE
  char* _serverMessage;
};

// функция инициализации окна
WINDOW* initWindow(enum tipElementForm tip);
  
// создание и заполнение структуры клиента
struct clientInfo* InitElementClient(int clientId, char* clientName, int connected);
  
int AddElementListClientInfoToEnd(struct listClientInfo ** list, int clientId, char* clientName, int connected);

// создание и заполнение структуры истории сообщения
struct message* InitElementMessage (int senderId, int receiverId, char* message );

int addElementListMessageToEnd(struct listMessages ** list, int senderId, int receiverId, char* message);

struct listClientInfo* initListClientInfo();

struct formElement* initFormElement(enum tipElementForm tip);

void displayContactInWindow(struct formElement* element);

void displayMessageHistoriInWindow(struct formElement* element);

void displayServiceMessegeInWindow(struct formElement* element);

void displayMessageInWindow(struct formElement* element);

void displayElementForm(struct formElement* element);

/* функция набора сообщения */
void writeMessage(struct formElement* elementMessageSet, char ch);

/* функция получения номера айди получатели из листа клиентов*/
int findNumbeReceverId(struct formElement* element);


#endif //__CLIENT__
