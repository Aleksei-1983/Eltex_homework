#include "Server_Client.h"


int main() {
  key_t key = ftok("shared_memory", 1234); 
  int shmid = shmget(key, SHM_SIZE, 0666); 

  if (shmid == -1) {
    perror("shmget");
    exit(1);
  }

  char *shm = (char *)shmat(shmid, NULL, 0); 

  if (shm == (char *)-1) {
    perror("shmat");
    exit(1);
  }
    
  key_t keySemun = ftok("shared_memory", 1);
  int semid = semget(keySemun, 1, 0666); 

  if (semid == -1) {
    perror("semget");
    exit(1);
  }
 
  
  char str[] = "Hi";
  int sem;
  while (1) {
    struct sembuf sb = {0, -1, IPC_NOWAIT}; 
    /*если функция semop() вернула -1 то изменение симофора не получилось он заблокирован если 
     *вернула 0 то с симофорам произошли изменения  */
    sem = semop(semid, &sb, 1); 
    printf("in sem = %d\n", sem);
    if (*shm == '\0') { 
      printf("no str\n");
      sprintf(shm, "%s", str); 
      printf("Client: Index written to shared memory: %s\n", shm);
    }
    else{
      printf("Server: Index written to shared memory: %s\n", shm);
      *shm = '\0';
    }
	
        
    sb.sem_op = 1; 
    sem = semop(semid, &sb, 1); 
    printf("out sem = %d\n", sem);
    sleep(1);
  }

  shmdt(shm); 

  return 0;
}
