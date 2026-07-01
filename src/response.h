#ifndef RESPONSE_H
#define RESPONSE_H

#include "http.h"

void send_home(int client_fd);
void send_user_agent(struct HttpRequest *req, int client_fd);
void send_echo(const char *s, int client_fd);

// static void handle_file();

#endif
