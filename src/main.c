#include "server.h"

#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <assert.h>
#include <bits/types.h>
#include <err.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#define PORT "3490"
#define BACKLOGS 10

int main() {
  setbuf(stdout, NULL);

  struct addrinfo hints, *res;
  struct sockaddr_storage client_addr;
  int sockfd, client_fd;
  int yes = 1;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  // get address info
  int gai = getaddrinfo(NULL, PORT, &hints, &res);
  if (gai != 0) {
    fprintf(stderr, "%s\n", gai_strerror(gai));
    return 1;
  }

  // create socket
  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sockfd == -1) {
    perror("server: socket");
    freeaddrinfo(res);
    return 1;
  }

  // prevents 'Address already in use' errors
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
    perror("server: setsockopt");
    freeaddrinfo(res);
    close(sockfd);
    return 1;
  }

  // bind socket
  if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
    perror("server: bind");
    freeaddrinfo(res);
    close(sockfd);
    return 1;
  }

  freeaddrinfo(res);

  // listen for incoming connection requests
  if (listen(sockfd, BACKLOGS) == -1) {
    perror("server: listen");
    close(sockfd);
  }

  while (1) {
    // accept client connection
    socklen_t addr_len = sizeof(client_addr);
    client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);

    if (client_fd == -1) {
      perror("server: accept");
      continue;
    }

    handle_client(client_fd);
  }

  close(sockfd);
  return 0;
}
