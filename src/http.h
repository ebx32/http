#ifndef HTTP_H
#define HTTP_H

#include <stddef.h>
#include <sys/types.h>

#define BUFF_SIZE 4096
#define MAX_HEADERS 128

struct HttpHeader {
    char *key;
    char *value;
};

struct Bstring;

struct HttpRequest {
    char *method;
    char *path;
    char *version;

    struct HttpHeader *headers;
    ssize_t headers_len;

    struct Bstring *body;

    char *_buffer;
    ssize_t _buffer_len;
};

void http_parse_request(char *request_buffer, struct HttpRequest *req);
char* http_get_header(struct HttpRequest *req, const char *header);

#endif
