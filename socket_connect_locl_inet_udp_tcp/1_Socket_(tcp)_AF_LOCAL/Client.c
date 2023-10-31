#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/mysocket"

int main() {
    int client_fd;
    struct sockaddr_un server_addr;
    char buf[256];

    // Create socket
    client_fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (client_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    server_addr.sun_family = AF_LOCAL;
    strncpy(server_addr.sun_path, SOCKET_NAME, sizeof(server_addr.sun_path) - 1);
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Send data to server
    strncpy(buf, "Hello, server!", sizeof(buf) - 1);
    if (sendto(client_fd, buf, sizeof(buf), 0, NULL, 0) == -1) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }

    // Close socket
    close(client_fd);

    return 0;
}
