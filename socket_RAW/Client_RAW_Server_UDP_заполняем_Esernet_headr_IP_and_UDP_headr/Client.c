#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netpacket/packet.h>
#include <net/if.h>


#define SRC_MAC "78:8c:b5:b8:9b:5c"
#define DST_MAC "24:fd:52:3d:a4:b0"
//#define DST_MAC "ff:ff:ff:ff:ff:ff"
#define SRC_PORT 7777
#define DST_PORT 8888
#define SRC_ADDR "192.168.43.206"
#define DST_ADDR "192.168.43.171"
#define PACKET_SIZE 128
#define PAYLOAD_SIZE 128 - (sizeof(struct ether_header) +  sizeof(struct iphdr) + sizeof(struct udphdr))

unsigned short in_cksum(unsigned short *addr, int len) {
    int sum = 0;
    short tmp;
    unsigned short *ptr = addr;
    unsigned short answer;
    

    for(int i = 0; i < (len / 2); i++){
      sum += *ptr;
      ptr++;
    }

    tmp = (sum >> 16);
    answer =  ~((sum & 0xffff) + tmp);
    return answer;
}

int main() {
  int tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + PAYLOAD_SIZE;
  printf("tot_len = %d\n", tot_len);
  
  // Create a raw socket
  int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (sock < 0) {
    perror("socket() failed");
    exit(1);
  }

  char packet[PACKET_SIZE];
  memset(packet, 0, PACKET_SIZE);

  struct ether_header* eh = (struct ether_header*)packet;
  struct iphdr *ip_header = (struct iphdr *)(packet + sizeof(struct ether_header)); // ставис 
  struct udphdr *udp_header = (struct udphdr *)(packet + sizeof(struct ether_header) + sizeof(struct iphdr));
  char *payload = packet + sizeof(struct ether_header) +  sizeof(struct iphdr) + sizeof(struct udphdr);

  // Fill in Ethernet header

  sscanf(SRC_MAC, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &eh->ether_shost[0], &eh->ether_shost[1], &eh->ether_shost[2], &eh->ether_shost[3], &eh->ether_shost[4], &eh->ether_shost[5]);
  sscanf(DST_MAC, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &eh->ether_dhost[0], &eh->ether_dhost[1], &eh->ether_dhost[2], &eh->ether_dhost[3], &eh->ether_dhost[4], &eh->ether_dhost[5]);
  eh->ether_type = htons(ETH_P_IP); /*The Ethernet type, which specifies the protocol of the payload.*/

  //printf("DST = %02x:%02x:%02x:n%02x:n%02x:n%02x\n", eh->ether_dhost[0], eh->ether_dhost[1], eh->ether_dhost[2], eh->ether_dhost[3], eh->ether_dhost[4], eh->ether_dhost[5]);
  
  // fill in the IP header
  ip_header->ihl = 5;  /* Это поле 4 бита, которое указывает длину заголовка IP в 32-битных словах. Минимальное значение для этого поля - 5,
			  что соответствует заголовку размером 20 байт. Максимальное значение - 15, что соответствует заголовку размером 60 байт.*/
  ip_header->version = 4; /* Это поле 4 бита, которое указывает версию протокола IP. Текущая версия - 4, которая является наиболее распространенной версией.*/
  ip_header->tos = 0;  /* Это поле 8 бит, которое указывает тип обслуживания. Оно используется для приоритезации пакетов
			  и может использоваться для целей качества обслуживания (QoS). стандартно 0 */ 
  ip_header->tot_len = htons(tot_len);//sizeof(struct iphdr) + sizeof(struct udphdr) + PAYLOAD_SIZE; /*Это поле 16 бит, которое указывает общую длину IP-пакета, включая заголовок и данные. MTU - размер восновном 1500 (чтобы не разбивались )*/
  ip_header->id = htons(12345); /*Это поле 16 бит, которое используется для идентификации фрагментов фрагментированного IP-пакета. */
  ip_header->frag_off = 0;      //то поле 16 бит, которое используется для указания смещения данных в фрагментированном IP-пакете. Offset - 13 бит (индекс восмибайтного фрагмента)*/
  ip_header->ttl = 64; /*Это поле 8 бит, которое указывает время жизни пакета. Оно уменьшается на единицу каждый раз, когда пакет проходит через маршрутизатор,
			 и пакет отбрасывается, когда TTL достигает нуля.*/
  ip_header->protocol = IPPROTO_UDP; /*Это поле 8 бит, которое указывает протокол, используемый в части данных IP-пакета. Общие значения включают TCP (6),
				       UDP (17) и ICMP (1).*/
  ip_header->saddr = inet_addr(SRC_ADDR); /* Это поле 32 бита, которое указывает исходный IP-адрес пакета.*/
  ip_header->daddr = inet_addr(DST_ADDR); /* Это поле 32 бита, которое указывает IP-адрес назначения пакета.*/
  ip_header->check = 0; /*Это поле 16 бит, которое используется для проверки целостности заголовка чек суммы расчитывается
			  тольго заголовка IP (при отправке оп AF_PACKET и протоколу ETH_P_ALL нужно заполнять в ручную)*/

  //ip_header->check = htons(~(ip_header->version + ip_header->ihl + ip_header->tos + ip_header->tot_len + ip_header->id + ip_header->frag_off + ip_header->ttl + ip_header->protocol + ip_header->saddr + ip_header->daddr));
  ip_header->check = in_cksum((unsigned short*)ip_header, sizeof(struct iphdr));
  
  printf("check_sum = %d\n", ip_header->check);
  // fill in the UDP header
  udp_header->source = htons(SRC_PORT); // порт отправителя
  udp_header->dest = htons(DST_PORT); // порт получателя
  udp_header->len = htons(sizeof(struct udphdr) + PAYLOAD_SIZE); // длина пакета
  udp_header->check = 0; // контрольная сумма (не используется)

  // fill in the payload
  strcpy(payload, "hello server");
  //*(payload + strlen(payload)) = 0xaf;

  // Set up the destination address
  struct sockaddr_ll dest_addr;
  memset(&dest_addr, 0, sizeof(dest_addr));
  dest_addr.sll_family = AF_PACKET;
  dest_addr.sll_protocol = htons(ETH_P_ALL);
  dest_addr.sll_ifindex  = if_nametoindex("wlx788cb5b89b5c"); 
  dest_addr.sll_halen = ETH_ALEN;
  memcpy(dest_addr.sll_addr, eh->ether_dhost, ETH_ALEN);

  printf("DST = %02x:%02x:%02x:%02x:%02x:%02x\n", eh->ether_dhost[0], eh->ether_dhost[1], eh->ether_dhost[2], eh->ether_dhost[3], eh->ether_dhost[4], eh->ether_dhost[5]);
  printf("SRC = %02x:%02x:%02x:%02x:%02x:%02x\n", eh->ether_shost[0], eh->ether_shost[1], eh->ether_shost[2], eh->ether_shost[3], eh->ether_shost[4], eh->ether_shost[5]);

  // Send the packet
  if ( sendto(sock, packet, sizeof(packet), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
    exit(1);
  }
  
  // wait for a response from the server
  struct sockaddr_in client_addr;
  int strLen;
  socklen_t client_len = sizeof(client_addr);
  memset(&dest_addr, 0, sizeof(client_addr));
  
  while(1){
    printf("begin\n");
    if ( (strLen = recvfrom(sock, packet, sizeof(packet), 0, (struct sockaddr *)&client_addr, &client_len)) < 0) {
      perror("recvfrom");
      exit(EXIT_FAILURE);
    }
    printf("end %d\n", ntohs(udp_header->source));
    printf("number bit = %d\n", strLen);
    if(ip_header->saddr == inet_addr(DST_ADDR) && udp_header->source == htons(DST_PORT)){
       printf("send = %d , recv = %d\n", ntohs(udp_header->source), htons(udp_header->dest));
      break;
    }
  }

 

  printf("%s\n", payload);
  
  // Close the socket
  close(sock);

  return 0;
}

