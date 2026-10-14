#ifndef RESPOND_H
#define RESPOND_H

#include "server.h"
#include <openssl/ssl.h>

//respond body 
typedef struct{
    //start line
    //header 
    char server[100];
    char date[100];
    char content_length[100];
    char content_type[50];
    //body 
    char body[100];
}HttpRespond;

void send_response_function(SSL *ssl, HttpRequest *req);
void error_response_function(SSL *ssl, RequestError error);
char *content_type_function(char *path);

#endif