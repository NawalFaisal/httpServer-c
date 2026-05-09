#ifndef RESPOND_H
#define RESPOND_H

#include "server.h"
#include <openssl/ssl.h>

#include "request.h"

void send_response_function(SSL *ssl, HttpRequest *req);
void error_response_function(SSL *ssl, RequestError error);
char *content_type_function(char *path);

#endif