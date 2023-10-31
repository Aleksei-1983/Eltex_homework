/* клиетн и сервер пишут или читают в разделяемую память. Если в памити чо то
 * записанно то кто первый в нее зашол тот читает и стерает после прочтения  */
#include "Server_Client.h"


int main() {
  /* Создаем разделяемую память*/
  key_t key = ftok("shared_memory", 1234); 
  int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666); 

  if (shmid == -1) {
    perror("shmget");
    exit(1);
  }

  char *shm = (char *)shmat(shmid, NULL, 0); 

  if (shm == (char *)-1) {
    perror("shmat");
    exit(1);
  }
  /* создаем симофор*/
  key_t keySemun = ftok("shared_memory", 1); 
  int semid = semget(keySemun, 1, IPC_CREAT | 0666); 

  if (semid == -1) {
    perror("semget");
    exit(1);
  }

  union semun arg;
  arg.val = 1;
  semctl(semid, 0, SETVAL, arg); 

  char str[] = "wold";
  while (1) {
    struct sembuf sb = {0, -1, 0}; 
    semop(semid, &sb, 1); 
    sleep(10);
    
    if (*shm == '\0') { 
      printf("no str\n");
      sprintf(shm, "%s", str); 
      printf("Server: Index written to shared memory: %s\n", shm);
    }
    else{
      printf("Server: Index written to shared memory: %s\n", shm);
      *shm = '\0';
    }
        
        
    sb.sem_op = 1; 
    semop(semid, &sb, 1); 
	
    sleep(10);
  }

  shmdt(shm); 

  shmctl(shmid, IPC_RMID, NULL); 

  semctl(semid, 0, IPC_RMID, arg);
    
  return 0;
}
