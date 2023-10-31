#ifndef SERVER_CLIENT
#define SERVER_CLIENT

#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>
#include "read_line.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define _BSD_SOURCE

#define BACKLOG 5
#define PORT_NUM "7777"

#define SIZ_STR 100 /* Размер строки */

#define ADDRSTRLEN (NI_MAXSERV + NI_MAXHOST + 10)

#endif 
