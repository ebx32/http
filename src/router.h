#ifndef ROUTER_H
#define ROUTER_H

#include <stddef.h>
#include <sys/types.h>

void http_route_request(struct HttpRequest *req, const int client_fd);

#endif
