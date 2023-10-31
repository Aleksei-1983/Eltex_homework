#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define SRC_PORT 12345
#define DST_PORT 8888
#define SRC_ADDR "127.0.0.1"
#define DST_ADDR "127.0.0.1"
#define PACKET_SIZE 128
#define PAYLOAD_SIZE 64


int main() {
  // Create a raw socket
  int sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (sock < 0) {
    perror("socket() failed");
    exit(1);
  }

  // Set up the destination address
  struct sockaddr_in dest_addr;
  memset(&dest_addr, 0, sizeof(dest_addr));
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(DST_PORT); // преобразуем число (порт) в формат сети ВИКИНДЕН1!!!!
  dest_addr.sin_addr.s_addr = inet_addr(DST_ADDR); //преобразуем адрес строку "127.0.0.1" в inet формат


  char packet[PACKET_SIZE];
  memset(packet, 0, PACKET_SIZE);

  struct udphdr *udp_header = (struct udphdr *)packet;
  char *payload = packet + sizeof(struct udphdr);

  struct iphdr* ip_header_recv = (struct iphdr*)packet;
  struct udphdr* udp_header_recv = (struct udphdr*)(packet + sizeof(struct iphdr)); 
  char* payload_recv = packet + sizeof(struct iphdr) + sizeof(struct udphdr);
  
  // fill in the UDP header
  udp_header->source = htons(SRC_PORT); // порт отправителя
  udp_header->dest = htons(DST_PORT); // порт получателя
  udp_header->len = htons(sizeof(struct udphdr) + PAYLOAD_SIZE); // длина пакета
  udp_header->check = 0; // контрольная сумма (не используется)

  // fill in the payload
  strcpy(payload, "hello server");
    
  // Send the packet
  if (sendto(sock, packet, sizeof(packet), MSG_WAITALL, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
    perror("sendto() failed");
    exit(1);
  }

  // wait for a response from the server
  struct sockaddr_in client_addr;
  int strLen;
  socklen_t client_len = sizeof(client_addr);
  memset(&dest_addr, 0, sizeof(client_addr));
  
  while(1){
    printf("begin\n");
    if ( (strLen = recvfrom(sock, packet, sizeof(packet), MSG_WAITALL, (struct sockaddr *)&client_addr, &client_len)) < 0) {
      perror("recvfrom");
      exit(EXIT_FAILURE);
    }

    if(udp_header_recv->source == htons(DST_PORT)){
      printf("send = %d , recv = %d\n", ntohs(udp_header_recv->source), htons(udp_header_recv->dest));
      break;
    }
  }

 
  printf("%s\n", payload_recv);
 
  close(sock);

  return 0;
}

