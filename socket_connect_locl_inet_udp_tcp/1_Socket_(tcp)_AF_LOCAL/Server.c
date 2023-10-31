#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/mysocket"

int main() {
    int server_fd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len;
    char buf[256];

    // Create socket
    server_fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Bind socket to address
    server_addr.sun_family = AF_LOCAL;
    strncpy(server_addr.sun_path, SOCKET_NAME, sizeof(server_addr.sun_path) - 1);
    unlink(SOCKET_NAME);
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Receive data from client
    client_len = sizeof(client_addr);
    if (recvfrom(server_fd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &client_len) == -1) {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }

    printf("Received message: %s\n", buf);

    // Close socket
    close(server_fd);

    return 0;
}
