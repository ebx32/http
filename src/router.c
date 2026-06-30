#include "http.h"
#include "response.h"

#include <netdb.h>
#include <string.h>

/* ========== SUPPORTED ROUTES ==========
 * /: send_home
 * /echo/<text>: send_echo
 * /index.html: send_index
 * 404: send_404
 * /files/<some_file>: send_file
 * /time: send_time
 * =====================================
 */

void http_route_request(struct HttpRequest *req, const int client_fd) {
    if (strcmp(req->path, "/") == 0) {
        send_home(client_fd);
    } else if (strcmp(req->path, "/user-agent") == 0) {
        send_user_agent(req, client_fd);
    } else if (strncmp(req->path, "/echo/", 6) == 0) {
        char *s = req->path + 6;
        send_echo(s, client_fd);
    } else if (strcmp(req->path, "/index.html") == 0) {
        send_index(client_fd);
    } else {
        send_404(client_fd);
    }
}
