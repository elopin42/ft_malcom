#include "libft/libft.h"
#include <stdio.h>          // printf, perror
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>         // exit, EXIT_FAILURE
#include <unistd.h>         // write
#include <sys/socket.h>     // socket, recvfrom
#include <arpa/inet.h>      // htons
#include <linux/if_packet.h> // AF_PACKET
#include <linux/if_ether.h>  // ETH_P_ARP
#include <ifaddrs.h>

struct ethheader {
    unsigned char       dsta[6];
    unsigned char       srca[6];
    uint16_t            type;
};

int main(int ac, char **av){
  
  if (getuid() != 0){
    write(2, "This program needs to be run as root.\n", 38);
    return 1;
  }

  if (ac != 5){
    write(2, "wrong number arguments\n", 23);
    return 1;
  }

  int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
  if (sockfd < 0)
    return 1;

  // 1 -> source ip  2 -> source mac adress 3 -> target ip 4 -> target mac adress
  char *data[3];
  for (int i = 0; i < 3; i++){
    data[i] = ft_strdup(av[i + 1]);
    if (!data[i])
      return(1);
  }


  unsigned char buffer[4096]; 

  ssize_t bytes_recv = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
  if (bytes_recv < 0) {
      perror("recvfrom failed\n");
      exit(EXIT_FAILURE);
  }

  struct ethheader *eth = (struct ethheader*)buffer;
  if (bytes_recv <= 0 || bytes_recv < (long long) sizeof(struct ethheader))
    return (printf("error bytes receiv\n"), 1);

  printf("Received %zd bytes\n", bytes_recv);
  printf("Received msg --> %s\n", buffer);

  char dst[18];
  char src[18];
  for (int i = 0; i < 6; i++) {
      sprintf(dst + i * 3, "%02x:", eth->dsta[i]);
      sprintf(src + i * 3, "%02x:", eth->srca[i]);
  }
  dst[17] = '\0';
  src[17] = '\0';
  printf("dst: %s src: %s ", dst, src);
  // if (!strcmp(dst, data[0]))
  //   sendto

  switch (ntohs(eth->type)) {
    case 0x0800:
        printf("IPv4\n");
        break;
    case 0x0806:
        printf("ARP\n");
        break;
    case 0x86DD:
        printf("IPv6\n");
        break;
    default:
        printf("unknown\n");
        break;
  }
  return 0;
}
