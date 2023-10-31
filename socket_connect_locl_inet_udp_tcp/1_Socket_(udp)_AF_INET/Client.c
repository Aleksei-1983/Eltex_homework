#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE 256
#define PORT 8888
#define IP_STR "127.0.0.1"

int main() {
    int sockfd;
    int index;
    struct sockaddr_in servaddr;
    char buffer[BUF_SIZE];
    strcpy(buffer, "HI i client");
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    for(index = 0; index < 1; index++) {
        printf("Enter message : ");
 
        sendto(sockfd, (const char*)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr*)&servaddr, sizeof(servaddr));
	socklen_t len = sizeof(servaddr);
	int n = recvfrom(sockfd, (char*)buffer, BUF_SIZE, MSG_WAITALL, (struct sockaddr*)&servaddr, &len);

        buffer[n] = '\0';
        printf("Server : %s\n", buffer);
    }

    close(sockfd);
    return 0;
}

