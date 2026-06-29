#include "http.h"
#include <assert.h>
#include <stdlib.h>
#include <strings.h>
#include <err.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

static void parse_request_line(char **parse_buffer, struct HttpRequest *req);
static void parse_request_headers(char **parse_buffer, struct HttpRequest *req);

// Public API

void http_parse_request(char *request_buffer, struct HttpRequest *req) {
    req->_buffer = request_buffer;
    char *parse_buffer = request_buffer;
    /*
     * 1. Parse HTTP request line
     * 2. Parse HTTP request headers
     */
    parse_request_line(&parse_buffer, req);
    parse_request_headers(&parse_buffer, req);
}

char* http_get_header(struct HttpRequest *req, const char *header) {
    assert(req != NULL);
    assert(header != NULL);

    for (size_t i = 0; i < req->headers_len; ++i){
        if (strcasecmp(header, req->headers[i].key) == 0) {
            return req->headers[i].value;
        }
    }
    return NULL;
}

// Private utility functions

static void parse_request_line(char **parse_buffer, struct HttpRequest *req) {
    req->method = strsep(parse_buffer, " ");
    req->path = strsep(parse_buffer, " ");
    req->version = strsep(parse_buffer, "\r");
    (*parse_buffer)++;
}

static void parse_request_headers(char **parse_buffer, struct HttpRequest *req) {
    req->headers = malloc(sizeof(struct HttpHeader) * MAX_HEADERS);
    req->headers_len = 0;

    for (size_t i = 0; i < MAX_HEADERS; ++i) {
        char *header_line = strsep(parse_buffer, "\r");
        (*parse_buffer)++;

        if (header_line[0] == '\0') {
            break;
        }

        // parse header line
        char *key = strsep(&header_line, ":");
        header_line++; // consume space
        char *value = strsep(&header_line, "\0");

        req->headers[i].key = key;
        req->headers[i].value = value;

        ++req->headers_len;
    }
}
