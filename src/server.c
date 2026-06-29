#include "server.h"
#include "http.h"
#include "router.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <assert.h>
#include <sys/socket.h>
#include <unistd.h>

void handle_client(int client_fd) {
    char request_buffer[BUFF_SIZE];
    struct HttpRequest req = {0};

    // recieve HTTP request
    ssize_t bytes_recvd = recv(client_fd, request_buffer, BUFF_SIZE - 1, 0);
    if (bytes_recvd == -1) {
        perror("server: recv");
        close(client_fd);
        return;
    }
    request_buffer[bytes_recvd] = '\0';

    // parse HTTP request
    http_parse_request(request_buffer, &req);

    // route requests and send reponse
    http_route_request(&req, client_fd);

    free(req.headers);
    close(client_fd);
}
