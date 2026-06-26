/*
 *\/
 *
 *
 *
 */

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
