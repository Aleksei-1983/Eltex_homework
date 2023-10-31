#include "Server.h"

int main() {
  int fileMk; // Именованный канал
  int serverFd;
  int dummyFd;
  int fileUnlink;
   
  
    int shmid; // Идентификатор разделяемой памяти
    int semid; // Идентификатор массива семафоров
    client_info_t *clients; // Указатель на структуру с информацией о клиентах
    message_t *message; // Указатель на структуру с информацией о сообщении
    struct address address; // получает по очереди FIFO отпровителя и получателя
    
    union semun arg;
    int i, j;
    int sender_id, receiver_id;
    char msg[MAX_MSG_LEN];

    key_t key = ftok("Server.c", 1);
    key_t keySemun = ftok("Server.c", 2);

    // Создаем разделяемую память для хранения информации о клиентах и сообщениях
    shmid = shmget(key, sizeof(client_info_t) * MAX_CLIENTS + sizeof(message_t), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // Получаем указатель на разделяемую память
    // (первым идет массив структур клиетов их может быть не болше 10)
    clients = (client_info_t *)shmat(shmid, NULL, 0);
    if ((void *)clients == (void *)(-1)) {
        perror("shmat");
        exit(1);
    }
    
    //Получаем указатель на общие сообщениясообщение 
    message = (message_t *)(clients + MAX_CLIENTS);

    // Инициализируем информацию о клиентах
    for (i = 0; i < MAX_CLIENTS; i++) {
        clients[i].client_id = i + 1;
        sprintf(clients[i].client_name, "Client %d", i +1);
        clients[i].connected = 0;
    }

    // Создаем массив семафоров
    semid = semget(keySemun, MAX_CLIENTS + 1, IPC_CREAT | 0666); 
    if (semid < 0) {
        perror("semget");
        exit(1);
    }

    // Инициализируем массив семафоров
    arg.val = 1;
    for (i = 0; i < (MAX_CLIENTS + 1); i++) {
      semctl(semid, i, SETVAL, arg); // SETVAL когда используется 4 аргумент то инициализирует занчением arg.val
    }

  //блокируем все симафоры не кто не может ни четать не писать в общую память
  for(i = 0; i < (MAX_CLIENTS + 1); i++){
    if (semop(semid, &(struct sembuf){.sem_num = i, .sem_op = -1}, 1) < 0) {
      perror("semop");
      exit(1);
    }
  }
   

  // Создает именнованый конал
  fileMk = mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP);

  if (fileMk == -1 && errno != EEXIST){
    printf("mkfifo %s\n", SERVER_FIFO);
    exit(EXIT_FAILURE);
  }

  serverFd = open(SERVER_FIFO, O_RDONLY);
  if(serverFd == -1 ){

    switch(errno){
      case ENOENT:
	printf("File %s does not exit\n", SERVER_FIFO);
        break;
	
      case EACCES:
	printf("No file %s\n permissions", SERVER_FIFO );
        break;
	
      default:
	printf("File %s eccess error\n", SERVER_FIFO);
	break;
    }
    exit(EXIT_FAILURE);
  }
  
  /*Открываем дополнительный записывающий дескриптор
   чтобы некогда не получать конец файла*/

  if ((dummyFd = open(SERVER_FIFO, O_WRONLY)) == -1){
    printf("open %s\n", SERVER_FIFO);
    exit(EXIT_FAILURE);
  }
  // конец создание именованной очереди 

  
  while (1) {
    printf("Hi");    
      // Получаем адрес от кого и камо напровляется сообщение 
      if(read(serverFd, &address , sizeof(struct address)) != sizeof(struct address)){
	printf("Error reading request\n");
	continue;   
      
      }
      /* Дальше сервер должен зделать для всех блакировку симофоров кроме кто должен писать*/
      printf("sender: %d, receiver: %d", address.sender_id, address.receiver_id);
      // Если клиент-получатель подключен, отправляем сообщение ему
      if (clients[address.receiver_id -1].connected) {
	// Устанавливаем семафор для клиента - устанавливает для клиента отправителя разблокировать
	if (semop(semid, &(struct sembuf){.sem_num = address.sender_id, .sem_op = 1, .sem_flg = 0}, 1) < 0) {
	  perror("semop");
	  exit(1);
	}
	
	// Ждем, пока клиент-получатель прочтет сообщение и разблокирует оповещение
	// о том что получил и мы его тут же заблокируем 
	if (semop(semid, &(struct sembuf){.sem_num = 0, .sem_op = -1, .sem_flg = 0}, 1) < 0) {
	  perror("semop");
	  exit(1);
	}
      }
      address.sender_id = 0;
      address.receiver_id = 0;
 
    }

     
  //удаляем очередь
  fileUnlink = unlink(SERVER_FIFO);

  if (fileUnlink == -1){
    printf("unlink %s\n", SERVER_FIFO);
    exit(EXIT_FAILURE);
  }
    
  // Удаляем разделяемую память и массив семафоров
  shmdt((void *)clients);
  shmctl(shmid, IPC_RMID, 0);
  semctl(semid, 0, IPC_RMID);

    return 0;
}

