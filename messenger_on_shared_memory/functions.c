#include "Client.h"

extern struct clientInfo *clients; // Указатель на структуру с информацией о клиентах
extern struct message *message; // Указатель на структуру в обещей память с информацией о сообщении
extern struct clientInfo myClientInfo;

/* функция генерирует размер окна
 * tip -> тип окна
 * возвращает указатель на окно*/
WINDOW* initWindow(enum tipElementForm tip)
{
  WINDOW* newWindow;
  
  switch (tip){

  case MESSAGE_HISTORY:
    newWindow = newwin((LINES / 3) * 2 , (COLS / 3) * 2, 0, 0);
    break;
  case MESSAGE_SET:
    newWindow = newwin((LINES / 3) -2 , (COLS / 3) * 2, (LINES / 3) * 2, 0);
    break;
  case CONTACT_LIST:
    newWindow = newwin(LINES -2, (COLS / 3) , 0, (COLS / 3) * 2);
    break;
  case SERVICE_MESSAGE:
    newWindow = newwin(2, COLS , LINES - 2, 0);
    break;
  }

  return newWindow;
}
      
/* создает (выделяет память) структуру с типом clientinfo и заполняет ее данными о клиенте
 * clientId -> номер который пресваевается сервером 
 * clientname -> ник нейм
 * connected -> в сети или не в сети
 * возвращает адрес на структур clientinfo;
 */
struct clientInfo* InitElementClient(int clientId, char* clientName, int connected)
{

  struct clientInfo* Ptr = malloc(sizeof(struct clientInfo));
  if(Ptr != NULL){
      Ptr->_clientId = clientId;
      Ptr->_connected = connected;
      strcpy(Ptr->_clientName , clientName);
  }

  return Ptr;
}

/* создает (выделяет память) структуру listClientinfo заполняет ее clientinfo и добовляет в канец списка 
 * list -> указатель на начало списка в котором хранятся информация о клиентах  
 * clientId -> номер который пресваевается сервером 
 * clientname -> ник нейм
 * connected -> в сети или не в сети
 * возвращает 0 если удолось добавить и -1 если нет
 */      
int AddElementListClientInfoToEnd(struct listClientInfo ** list, int clientId, char* clientName, int connected )
{
  struct clientInfo* newElement = InitElementClient(clientId, clientName, connected);
  struct listClientInfo* newElementList = malloc(sizeof(struct listClientInfo));
  struct listClientInfo* elementList; 
  
  if(newElementList != NULL && newElement != NULL){
    newElementList->_clientInfo = newElement;
    newElementList->_nexListClientInfo = NULL;
    
    if(*list != NULL){
      
      elementList = *list;
      
      while(elementList->_nexListClientInfo != NULL){
	elementList = elementList->_nexListClientInfo;
      }
      elementList->_nexListClientInfo = newElementList;
      
    }else{
      *list = newElementList;
    }
    return 0;
  }
  else{
    return -1;
  }
}

/* создает (выделяет память) структуру с типом message и заполняет ее данными 
 * senderid -> номер отпровителя 
 * receiverid -> номер получателя
 * message -> сообщение
 * возвращает адрес на структур message;
 */
struct message* InitElementMessage (int senderId, int receiverId, char* message )
{

  struct message* Ptr = malloc(sizeof(struct message));
  if(Ptr != NULL){
      Ptr->_senderId = senderId;
      Ptr->_receiverId = receiverId;
      strcpy(Ptr->_message , message);
  }

  return Ptr;
}

/* создает (выделяет память) структуру с типом listmessages и заполняет ее данными
 * и добовляет ее в конец списка
 * list -> указатель на начало списка
 * senderid -> номер отпровителя 
 * receiverid -> номер получателя
 * message -> сообщение
 * возврашает адрес на структур message;
 */
int addElementListMessageToEnd(struct listMessages ** list, int senderId, int receiverId, char* message )
{
  struct message* newElement = InitElementMessage(senderId, receiverId, message);
  struct listMessages* newElementList = malloc(sizeof(struct listMessages));
  struct listMessages* elementList; 
  
  if(newElementList != NULL && newElement != NULL){
    newElementList->_message = newElement;
    newElementList->_nexListMessages = NULL;
    
    if(*list != NULL){
      
      elementList = *list;
      
      while(elementList->_nexListMessages != NULL){
	elementList = elementList->_nexListMessages;
      }
      elementList->_nexListMessages = newElementList;
      
    }else{
      *list = newElementList;
    }
    return 0;
  }
  else{
    return -1;
  }
} 

/* создает список клиентов
 * возвращает указатель на начало списка  */
struct listClientInfo* initListClientInfo()
{
struct listClientInfo* list;
   int index;

   for(index = 0; index < MAX_CLIENTS; index++){
     if(clients[index]._clientId != myClientInfo._clientId){
       if(AddElementListClientInfoToEnd(&list, clients[index]._clientId, clients[index]._clientName, clients[index]._connected) == -1){
       printf("eror init list client");
       exit(EXIT_FAILURE);
       }
     }
   }
   return list;
 }

/* создает элемент формы в зависимости от типа перереданного элемента
 * tip -> тип элемента формы
 * возвращает указатнль на структуру formelement  
 */
struct formElement* initFormElement(enum tipElementForm tip)
{
  int index; 
  struct formElement* element = malloc(sizeof(struct formElement));
  if(element == NULL){
    return NULL;
  }
  element->_tip = tip;
  element->_wnd = initWindow(tip);
  element->_curOnClient = 0;
  switch (tip){  
  case MESSAGE_HISTORY:
    element->_listClient = NULL;
    element->_message = NULL;
    element->_listMessagesHistory = NULL;    
    element->_serverMessage = NULL;
    break;
  case MESSAGE_SET:
    element->_listClient = NULL;
    element->_message = malloc(MAX_MSG_LEN);
    element->_message[0] = '\0';
    element->_listMessagesHistory = NULL;    
    element->_serverMessage = NULL ;
    break;
  case CONTACT_LIST:
    element->_listClient = initListClientInfo();
    element->_message = NULL;
    element->_listMessagesHistory = NULL;    
    element->_serverMessage = NULL ;
    element->_numClient = MAX_CLIENTS - 2;
    break;
  case SERVICE_MESSAGE:
    element->_listClient = NULL;
    element->_message = NULL;
    element->_listMessagesHistory = NULL;    
    element->_serverMessage = malloc(MAX_MSG_LEN);
    break;

  } 

  return element;
}

/* отображает список клиентов на экран
 *element -> указатель на элемент формы отображение клиентов
 **/
void displayContactInWindow(struct formElement* element)
{
  struct listClientInfo* listElement;
  int index = 0;
  if(element == NULL){
    exit(EXIT_FAILURE);
  }
  listElement = element->_listClient;
  wclear(element->_wnd);

  while(listElement != NULL){
   if(index == element->_curOnClient){
     wattron(element->_wnd, A_REVERSE);

     mvwprintw(element->_wnd, index, 0, "onl: %d,  Id: %d, %s",
		 listElement->_clientInfo->_connected,
		 listElement->_clientInfo->_clientId,
		 listElement->_clientInfo->_clientName); // Вывод имени файла в первое окно
       
     wattroff(element->_wnd, A_REVERSE);
     listElement = listElement->_nexListClientInfo;
     index++;
       
   }
   else{
     mvwprintw(element->_wnd, index, 0, "onl: %d,  Id: %d, %s",
		 listElement->_clientInfo->_connected,
		 listElement->_clientInfo->_clientId,
		 listElement->_clientInfo->_clientName);

     listElement = listElement->_nexListClientInfo;
     index++;
   }
  }
  wrefresh(element->_wnd);
}

/* отображает историю сообщений на экран
 *element -> указатель на элемент формы отображение истории сообщений
 **/
void displayMessageHistoriInWindow(struct formElement* element)
{
  struct listMessages* listElement;
  int index = 0;
  if(element == NULL){
    exit(EXIT_FAILURE);
  }
   
  if(message->_message[0] != '\0'){
    if(addElementListMessageToEnd(&element->_listMessagesHistory, message->_senderId, message->_receiverId, message->_message) == -1){
    printf("eror add list message ");
    exit(EXIT_FAILURE);
    }
  }

  listElement = element->_listMessagesHistory;
  wclear(element->_wnd);

  while(listElement != NULL){
    if(myClientInfo._clientId == listElement->_message->_senderId){
      mvwprintw(element->_wnd, index, 0, "my message: %s", myClientInfo._clientName);
    }
    else{
      mvwprintw(element->_wnd, index, 0, "sender: %d", listElement->_message->_senderId);
    }
     
    index++;
    mvwprintw(element->_wnd, index, 0, "message: %s", listElement->_message->_message);

    index += 2;
    listElement = listElement->_nexListMessages;
  }
  
  wrefresh(element->_wnd);
}

/* отображает набираемое сообщение в поле ввода
 *element -> указатель на элемент формы отображение поле ввода сообщения
 **/
void displayServiceMessegeInWindow(struct formElement* element)
{
  if(element->_serverMessage == NULL){
    exit(EXIT_FAILURE);
  }

  wclear(element->_wnd);
  mvwprintw(element->_wnd, 1, 0, "%s", element->_serverMessage);
  wrefresh(element->_wnd);
}

/* отображает служебную инфорацию
 *element -> указатель на элемент формы отображение служебной информации
 **/
void displayMessageInWindow(struct formElement* element)
{
  if(element->_message == NULL){
    exit(EXIT_FAILURE);
  }

  wclear(element->_wnd);
  mvwprintw(element->_wnd, 1, 0, "%s", element->_message);
  wrefresh(element->_wnd);
}

/* отображает элемент формы на экран
 *element -> указатель на элемент формы 
 **/
void displayElementForm(struct formElement* element)
{
   
  switch (element->_tip){  
  case MESSAGE_HISTORY:
    displayMessageHistoriInWindow(element);
    break;
  case MESSAGE_SET:
    displayMessageInWindow(element);
    break;
  case CONTACT_LIST:
    displayContactInWindow(element);
    break;
  case SERVICE_MESSAGE:
    displayServiceMessegeInWindow(element);
    break;
  }
}

/* функция набора сообщения
 * elementmessageset -> указатель на элемент формы  ввод сообщения 
 * ch -> символ
 */

void writeMessage(struct formElement* elementMessageSet, char ch)
{  
 
  int messageLen = strlen(elementMessageSet->_message);
  
  if( messageLen < (MAX_MSG_LEN -1) &&((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '.') || (ch == ' ') || (ch == KEY_BACKSPACE))){

    if(ch == KEY_BACKSPACE){
      if(messageLen <= 0){
	return;
      }
    elementMessageSet->_message[messageLen - 1] = '\0';
    }
    else{
      elementMessageSet->_message[messageLen] = (char)ch;
      elementMessageSet->_message[messageLen + 1]  = '\0';
    }
  }
    

  displayElementForm(elementMessageSet);
}

/* функция получения номера клиента на котором установлен курсор
 * element -> элемент формы
 * возвращает номер клиента*/
int findNumbeReceverId(struct formElement* element)
{
  int cursor = element->_curOnClient;
  int index = 0;
  struct listClientInfo* listElement;

  if(element == NULL){
    exit(EXIT_FAILURE);
  }

  listElement = element->_listClient;
  while(listElement != NULL){
    if((index == cursor) && (myClientInfo._clientId != listElement->_clientInfo->_clientId)){
      return listElement->_clientInfo->_clientId;
    }
    
    index++;
    listElement = listElement->_nexListClientInfo;
  }
}

