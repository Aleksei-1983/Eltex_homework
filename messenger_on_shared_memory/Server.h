#ifndef __SERVER__
#define __SERVER__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SERVER_FIFO "/tmp/seqnum_sv" /*имя серверной очереди */
#define MAX_CLIENTS 10 /* поправить клиентов должно быть 10 а семофоров 11 нулевой семофор занит сервером*/
#define MAX_MSG_LEN 100

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};


// Структура для хранения информации о клиентах
typedef struct {
    int client_id;
    char client_name[MAX_MSG_LEN];
    int connected;
} client_info_t;

// Структура для хранения информации о сообщении
typedef struct {
    int sender_id;
    int receiver_id;
    char message[MAX_MSG_LEN];
} message_t;

struct address {
   int sender_id;
   int receiver_id; 
};


#endif //__SERVER__
