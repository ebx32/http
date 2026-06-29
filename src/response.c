#include "http.h"
#include "bstring.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

void send_home(int client_fd) {
    const char response[] = "HTTP/1.1 200 OK\r\n\r\n";
    send(client_fd, response, strlen(response), 0);
}

void send_user_agent(struct HttpRequest *req, const int client_fd) {
    int bytes_sent;

    char *user_agent = http_get_header(req, "user-agent");

    if (user_agent == NULL) {
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
}
