#include "server.h"
#include "http.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <sys/socket.h>
#include <unistd.h>

void handle_client(int client_fd) {
    char request_buffer[BUFF_SIZE];
    struct HttpRequest req = {0};

    ssize_t bytes_recvd = recv(client_fd, request_buffer, BUFF_SIZE - 1, 0);
    if (bytes_recvd == -1) {
        perror("server: recv");
        close(client_fd);
        return;
    }
    request_buffer[bytes_recvd] = '\0';

    // parse HTTP request
    http_parse_request(request_buffer, &req);

    // get content-length
    size_t content_length = 0;
    char *content_length_header = http_get_header(&req, "content-length");
    if (content_length_header != NULL) {
        content_length = strtol(content_length_header, NULL, 10);
    }
    printf("%zu\n", content_length);

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
    } else if (strcmp(req.path, "/index.html") == 0) {
        FILE *fd = fopen("public/index.html", "r");
        if (fd == NULL) {
            FILE *not_found = fopen("public/404.html", "r");
        }
    }

    free(req.headers);
    close(client_fd);
}
