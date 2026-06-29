#ifndef RESPONSE_H
#define RESPONSE_H

void send_home(int client_fd);
void send_user_agent(struct HttpRequest *req, const int client_fd);

#endif
