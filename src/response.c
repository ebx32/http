#include "response.h"
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

void send_user_agent(struct HttpRequest *req, int client_fd) {
    char *response = malloc(BUFF_SIZE);
    char *user_agent = http_get_header(req, "user-agent");
    if (user_agent == NULL) {
        user_agent = "NULL";
    }

    sprintf(response,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length:%zu\r\n\r\n"
            "%s",
            strlen(user_agent), user_agent);
    send(client_fd, response, strlen(response), 0);
    free(response);
}

void send_echo(const char *s, int client_fd) {
    char *response = malloc(BUFF_SIZE);

    sprintf(response,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length:%zu\r\n\r\n"
        "%s",
        strlen(s), s);
    send(client_fd, response, strlen(response), 0);
    free(response);
}

// static functions
