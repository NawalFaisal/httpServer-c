#ifndef REQUEST_H
#define REQUEST_H

#include "server.h"
#include <openssl/ssl.h>
#include "httpstatus.h"


typedef struct {
    char method[10];
    char path[100];
    char version[10];
    char host[100];
    char content_type[100];
    char user_agent[200];
    char accept[100];
    int  content_length;
    char body[BUFFER_SIZE];
    HttpStatus status;

}HttpRequest;

HttpRequest parse_request_function(SSL *ssl);


#endif 