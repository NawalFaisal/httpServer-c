#ifndef RESPOND_H
#define RESPOND_H

#include "server.h"
#include "request.h"
#include "httpstatus.h"
#include <openssl/ssl.h>

void send_response_function(SSL *ssl, HttpRequest *req);
void error_response_function(SSL *ssl, HttpStatus status);
char *content_type_function(char *path);

#endif