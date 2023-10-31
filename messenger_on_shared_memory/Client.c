/* программа является Клиентом чата который взаимодействует с сервером по средствам именованных каналов
 * и разделяемой память.
 * Описание работы -> подключается к общей память и именнованованному каналу что создал сервер,
 * регестрируется на сервере что подключился,
 * создает форму м заполняет ее данными о клиентах из разделяемой память где хранится инфо о клиентах,
 * отправка сообщения -> когда клиенту нужно отправить сообщение другому клиенту он формерует запрос к серверу
 * в именованный канал что я такойта хочу отправить такомуто сообщение сервер для отправителя разблокирует
 * общую область памяти для записи сообщения отправителю когда сообщение записанно клиент разблокирует получателя
 * и получатель смотрит если там есть сообщение то он его читает и удаляет его и сообщает серверу о том что сообщение прочитано
 * и сервер начинает обрабатывать следующий запрос от клиента на отправку сообщения.*/
#include "Client.h"

struct clientInfo *clients; // Указатель на структуру с информацией о клиентах
struct message *message; // Указатель на структуру в обещей память с информацией о сообщении
struct clientInfo myClientInfo;

  
int main() {

  struct formElement* form[NUM_ELEMENT_FORM];
  
  int fileMk; // Именованный канал
  int serverFd;
  int dummyFd;
  
  int shmid; // Идентификатор разделяемой памяти
  int semid; // Идентификатор массива семафором
  int sem;

  struct address address; // то что надо отправить серверу 

  int ch;
  int client_id;
  int index, i, j, k;

  key_t key = ftok("Server.c", 1); 
  key_t keySemun = ftok("Server.c", 2); 

  // Инициализация библиотеки curse
  initscr(); // Инициализация curses и создание окна stdscr

  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  
  cbreak(); // Переключение в неканонический режим ввода
  noecho(); // Отключение эхо-режима
  curs_set(0);// скрываем курсов 
  keypad(stdscr, TRUE); // Включение обработки функциональных клавиш
  timeout(100);
  refresh();

  // Получаем идентификатор разделяемой памяти созданной сервером
  shmid = shmget(key, sizeof(struct clientInfo) * MAX_CLIENTS + sizeof(struct message), 0666);
  if (shmid < 0) {
    perror("shmget");
    exit(1);
  }

  // Получаем указатель на разделяемую память клиенты и сообщения 
  clients = (struct clientInfo *)shmat(shmid, NULL, 0);
  if ((void *)clients == (void *)(-1)) {
    perror("shmat");
    exit(1);
  } 
  message = (struct message *)(clients + MAX_CLIENTS);

  //присваивание id клиенту  
  for(k = 0; k < MAX_CLIENTS; k++){
    if(clients[k]._connected == 0){
      myClientInfo._clientId = clients[k]._clientId;
      strcpy(myClientInfo._clientName, clients[k]._clientName);
      myClientInfo._connected = 1;
      clients[k]._connected = 1;
      break;
    } 
  }  

  // инициируем форму со списком контактов и выделаем память под массивы строк
  for(index = 0; index < NUM_ELEMENT_FORM; index++ ){
    form[index] = initFormElement(index);
    wbkgd(form[index]->_wnd, COLOR_PAIR(1));
  }

  char* str = ":";
  //strcpy(form[MESSAGE_SET]->_message, str);
  strcpy(form[SERVICE_MESSAGE]->_serverMessage, myClientInfo._clientName);
  
  // Создаем массив семафоров мы должны тут подключатся
  //а не создавать симафоры так как сервер уже создал ВАЖНО!!!!
    
  semid = semget(keySemun, MAX_CLIENTS + 1, 0666);
  if (semid < 0) {
    perror("semget");
    exit(1);
  }
  
  /*Открываем FIFO на записть который слушает сервер */
  serverFd = open(SERVER_FIFO, O_WRONLY);
  if(serverFd == -1){
    printf("mkfifo %s\n", SERVER_FIFO);
    exit(EXIT_FAILURE);
  }
  
  // выводим форму на экран
  for(index = 0; index < NUM_ELEMENT_FORM; index++){
    displayElementForm(form[index]);
  }
    
  // Входим в цикл обработки сообщений
  while ((ch = getch()) != 'q') {

    writeMessage(form[MESSAGE_SET], ch);

    switch(ch){
    case KEY_RIGHT:
      /* заполняем адреса отпровителя и получателя*/
      address._senderId = myClientInfo._clientId;
      address._receiverId = findNumbeReceverId(form[CONTACT_LIST]);
      /*Передаем серверу запрос по FIFO о том что у нас есть сообщение */
      if(write(serverFd, &address, sizeof(struct address)) != sizeof(struct address)){
	printf("write");
	exit(EXIT_FAILURE);
      } 
      break;
    case KEY_UP:
      if(form[CONTACT_LIST]->_curOnClient > 0 ){
	form[CONTACT_LIST]->_curOnClient--;
	displayElementForm(form[CONTACT_LIST]);
      }
      break;

    case KEY_DOWN:
      if(form[CONTACT_LIST]->_curOnClient < form[CONTACT_LIST]->_numClient){
	form[CONTACT_LIST]->_curOnClient++;
	displayElementForm(form[CONTACT_LIST]);
      }
      break;
    case KEY_BACKSPACE:/* если передовать спец клавиши в типе char в функцию там их не видно */
      strcpy(form[MESSAGE_SET]->_message, str);
      break;
    }

    // пробуем войти в память если сервер разблокировал симофор и   блакируем и ощуществляем
    // запись или чтения с памяти в зависимости есть там сообшения или нет
    sem = semop(semid, &(struct sembuf){.sem_num = myClientInfo._clientId, .sem_op = -1, .sem_flg = IPC_NOWAIT}, 1);

    /* если sem == 0 серввер разблокиовал этого клиента для записи в разделяемую память*/
    if(sem == 0){

      // если в сообщение символ конца строки первый значит сообщения там нет и будим его туда писать мы отправитель
      if(message->_message[0] == '\0'){
	/* корпируем структуру с сообщением в общую память */
	struct message bufMessage;
	bufMessage._senderId = address._senderId;
	bufMessage._receiverId = address._receiverId;
	strcpy(bufMessage._message, form[MESSAGE_SET]->_message);
        memcpy(message, &bufMessage, (sizeof(struct message)));

	/* читаем собственное сообщение из разделяемой память */
	displayMessageHistoriInWindow(form[MESSAGE_HISTORY]);
	
        /* разблокируем получателя сообщения*/
        sem = semop(semid, &(struct sembuf){.sem_num = address._receiverId, .sem_op = 1, .sem_flg = IPC_NOWAIT}, 1);
	if(sem == -1){
          perror("semop");
          exit(EXIT_FAILURE);
        }
      }
      else{//сообщение там есть и мы будем его от туда читат мы получатель
	/* читаем сообщение из разделяемой память */
	displayMessageHistoriInWindow(form[MESSAGE_HISTORY]);
	message->_message[0] = '\0';

	/* разблокируем нуливой симафор (перезаем серверу о том что мы почитале сообщения и разблокируем сервер)*/
        if (semop(semid, &(struct sembuf){.sem_num = 0, .sem_op = 1 , .sem_flg = 0}, 1) < 0) {
	  perror("semop1");
	  exit(1);
        }
      }
    }
    else if(sem < 0){

    }
 
  }

  refresh();
  endwin();
  return 0;
}

