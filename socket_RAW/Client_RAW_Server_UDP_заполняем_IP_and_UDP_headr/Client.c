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

  // Set socket options
  int optval = 1;
  if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(optval)) < 0) {
    perror("setsockopt() failed");
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

  struct iphdr *ip_header = (struct iphdr *)packet; // ставис 
  struct udphdr *udp_header = (struct udphdr *)(packet + sizeof(struct iphdr));
  char *payload = packet + sizeof(struct iphdr) + sizeof(struct udphdr);

  // fill in the IP header
  ip_header->ihl = 5;  /* Это поле 4 бита, которое указывает длину заголовка IP в 32-битных словах. Минимальное значение для этого поля - 5,
			            что соответствует заголовку размером 20 байт. Максимальное значение - 15, что соответствует заголовку размером 60 байт.*/
  ip_header->version = 4; /* Это поле 4 бита, которое указывает версию протокола IP. Текущая версия - 4, которая является наиболее распространенной версией.*/
  ip_header->tos = 0;  /* Это поле 8 бит, которое указывает тип обслуживания. Оно используется для приоритезации пакетов
			  и может использоваться для целей качества обслуживания (QoS). стандартно 0 */ 
  ip_header->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + PAYLOAD_SIZE; /*Это поле 16 бит, которое указывает общую длину IP-пакета, включая заголовок и данные. MTU - размер восновном 1500 (чтобы не разбивались )*/
  ip_header->id = htons(12345); /*Это поле 16 бит, которое используется для идентификации фрагментов фрагментированного IP-пакета. */
  ip_header->frag_off = 0; /*Это поле 16 бит, которое используется для указания смещения данных в фрагментированном IP-пакете. Offset - 13 бит (индекс восмибайтного фрагмента)*/
  ip_header->ttl = 64; /*Это поле 8 бит, которое указывает время жизни пакета. Оно уменьшается на единицу каждый раз, когда пакет проходит через маршрутизатор,
			           и пакет отбрасывается, когда TTL достигает нуля.*/
  ip_header->protocol = IPPROTO_UDP; /*Это поле 8 бит, которое указывает протокол, используемый в части данных IP-пакета. Общие значения включают TCP (6),
				            UDP (17) и ICMP (1).*/
  ip_header->saddr = inet_addr(SRC_ADDR); /* Это поле 32 бита, которое указывает исходный IP-адрес пакета.*/
  ip_header->daddr = inet_addr(DST_ADDR); /* Это поле 32 бита, которое указывает IP-адрес назначения пакета.*/
  ip_header->check = 0; /*Это поле 16 бит, которое используется для проверки целостности заголовка чек суммы расчитывается тольго заголовка IP*/

  
  // fill in the UDP header
  udp_header->source = htons(SRC_PORT); // порт отправителя
  udp_header->dest = htons(DST_PORT); // порт получателя
  udp_header->len = htons(sizeof(struct udphdr) + PAYLOAD_SIZE); // длина пакета
  udp_header->check = 0; // контрольная сумма (не используется)

  // fill in the payload
  strcpy(payload, "hello server");
  //*(payload + strlen(payload)) = 0xaf;
    
    
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
    printf("end %d\n", ntohs(udp_header->source));
    printf("number bit = %d\n", strLen);
    if(ip_header->saddr == inet_addr(DST_ADDR) && udp_header->source == htons(DST_PORT)){
      break;
    }
  }

 

  printf("%s\n", payload);
  
  // Close the socket
  close(sock);

  return 0;
}

