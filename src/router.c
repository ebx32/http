#include "http.h"
#include "response.h"

#include <netdb.h>
#include <string.h>
#include <stdio.h>

void http_route_request(struct HttpRequest *req, const int client_fd) {
    if (strcmp(req->path, "/") == 0) {
        send_home(client_fd);
    } else if (strcmp(req->path, "/user-agent") == 0) {
        send_user_agent(req, client_fd);
    } else if (strncmp(req->path, "/echo/", 6) == 0) {
        char *s = req->path + 6;
        send_echo(s, client_fd);
    } else if (strcmp(req->path, "/index.html") == 0) {
        // send_index(client_fd);
        printf("%s\n", req->path);
    } else if (strcmp(req->path, "/about.html") == 0) {
        // send_about();
        printf("%s\n", req->path);
    } else if (strncmp(req->path, "/files/", 7) == 0) {
        // send_file();
        printf("file\n");
    } else if (strcmp(req->path, "/time") == 0) {
        // send_time();
        printf("time\n");
    } else {
        // send_404(client_fd);
        printf("404\n");
    }
}
