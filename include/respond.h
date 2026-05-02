#ifndef RESPOND_H
#define RESPOND_H

#include "server.h"

#include "request.h"

void send_response_function(int client_fd, HttpRequest *req);
void error_response_function(int client_fd, int status_code, char *message);
char *content_type_function(char *path);

#endif