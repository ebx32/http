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
    struct HttpResponse res = {0};

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

    // route requests and send reponse
    route_request(&req, &res);

    free(req.headers);
    close(client_fd);
}
