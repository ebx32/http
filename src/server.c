#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <assert.h>
#include <bits/types.h>
#include <err.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#define PORT "3490"
#define BACKLOGS 10
#define BUFF_SIZE 4096
#define MAX_HEADERS 128

struct HttpHeader {
  char *key;
  char *value;
};

// response bode
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

struct Bstring bstring_init() {

};

struct Bstring bstring_append() {

};

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

char* http_get_header(struct HttpRequest *req, char *header) {
  assert(req != NULL);
  assert(header != NULL);

  for (size_t i = 0; i < req->headers_len; ++i){
    if (strcasecmp(header, req->headers[i].key) == 0) {
      return req->headers[i].value;
    }
  }
  return NULL;
}

void handle_client(int client_fd) {
  char request_buffer[BUFF_SIZE];
  size_t content_length = 0;

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

  // get content-length
  char *content_length_header = http_get_header(&req, "content-length");
  if (content_length_header != NULL) {
    content_length = strtol(content_length_header, NULL, 10);
  }
  printf("%d\n", (int)content_length);

  // send reponse
  if (strcmp(req.path, "/") == 0) {
    const char response[] = "HTTP/1.1 200 OK\r\n\r\n";
    send(client_fd, response, strlen(response), 0);
  } else if (strcmp(req.path, "/user-agent") == 0) {
    int bytes_sent = -1;
    // return User-Agent in reponse body
    char *user_agent = http_get_header(&req, "user-agent");
    if (user_agent == NULL){
      user_agent = "NULL";
    }

    char *response = malloc(BUFF_SIZE);
    sprintf(response,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length:%zu\r\n\r\n"
            "%s",
            strlen(user_agent), user_agent);
    bytes_sent = send(client_fd, response, strlen(response), 0);
    printf("%d\n", bytes_sent);
    free(response);
  // send webpages
  } else if (strcmp(req.path, "/index.html") == 0) {
    FILE *fd = fopen("public/index.html", "r");
    if (fd == NULL) {
      FILE *not_found = fopen("public/404.html", "r");
    }
  }

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
