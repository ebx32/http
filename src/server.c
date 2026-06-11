#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <bits/types.h>
#include <err.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT "3490"
#define BACKLOGS 10
#define BUFF_SIZE 4096
#define MAX_HEADERS 128

struct HttpHeader {
  char *key;
  char *value;
};

struct Bstring {
  char *data;
  size_t length;
  size_t capacity;
};

struct HttpRequest {
  char *method;
  char *path;
  char *version;

  // headers
  struct HttpHeader *headers;
  ssize_t headers_len;

  // request buffer
  char *_buffer;
  ssize_t _buffer_len;

  // body
  struct Bstring *body;
};

void parse_request() {
  
}

void parse_headers(char *parse_buffer, struct HttpRequest *req) {
  req->headers = malloc(sizeof(struct HttpHeader) * MAX_HEADERS);
  req->headers_len = 0;

  for (size_t i = 0; i < MAX_HEADERS; ++i) {
    char *header_line = strsep(&parse_buffer, "\r");
    parse_buffer++;

    if (header_line[0] == '\0') {
      break;
    }

    // parse header line
    char *key = strsep(&header_line, ":");
    header_line++; // consume space
    char *value = strsep(&header_line, "\0");

    req->headers[i].key = key;
    req->headers[i].value = value;

    ++req->headers_len;
  }
}

void parse_body() {
  
}

void handle_client(int client_fd) {

  char request_buffer[BUFF_SIZE];

  // recieve HTTP request
  ssize_t bytes_recvd = recv(client_fd, request_buffer, BUFF_SIZE - 1, 0);
  if (bytes_recvd == -1) {
    perror("server: recv");
    close(client_fd);
    return;
  }
  request_buffer[bytes_recvd] = '\0';

  // parse HTTP request line
  struct HttpRequest req = {0};
  req._buffer = request_buffer;
  char *parse_buffer = req._buffer;

  req.method = strsep(&parse_buffer, " ");

  if (strncmp(req.method, "GET", 3) == 0) {
    printf("Recieved HTTP GET Request!\n");
  }

  req.path = strsep(&parse_buffer, " ");
  req.version = strsep(&parse_buffer, "\r");
  parse_buffer++;

  // parse headers
  parse_headers(parse_buffer, &req);

  // get 'Content-Length' header and parse_body();
  

  // send response

  free(req.headers);
  close(client_fd);
}

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

  int gai = getaddrinfo(NULL, PORT, &hints, &res);
  if (gai != 0) {
    fprintf(stderr, "%s\n", gai_strerror(gai));
    return 1;
  }

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sockfd == -1) {
    perror("server: socket");
    freeaddrinfo(res);
    return 1;
  }

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
    perror("server: setsockopt");
    freeaddrinfo(res);
    close(sockfd);
    return 1;
  }

  if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
    perror("server: bind");
    freeaddrinfo(res);
    close(sockfd);
    return 1;
  }

  freeaddrinfo(res);

  if (listen(sockfd, BACKLOGS) == -1) {
    perror("server: listen");
    close(sockfd);
  }

  while (1) {
    socklen_t addr_len = sizeof(client_addr);
    client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);

    if (client_fd == -1) {
      perror("server: accept");
      continue;
    }

    // handle_client(client_fd, &client_addr);
    handle_client(client_fd);
  }

  close(sockfd);
  return 0;
}
