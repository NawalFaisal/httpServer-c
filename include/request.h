#ifndef REQUEST_H
#define REQUEST_H

#include "server.h"


typedef enum{
    REQUEST_OK,   
    REQUEST_BAD,
    REQUEST_UNAUTHORIZED,
    REQUEST_FORBIDDEN,
    REQUEST_NOT_FOUND,
    REQUEST_METHOD_NOT_ALLOWED,
    REQUEST_PAYLOAD_TOO_LARGE,
    REQUEST_INTERNAL_SERVER_ERROR,
    REQUEST_SERVICE_UNAVAILABLE
    
}RequestError;

typedef struct {
    char method[10];
    char path[100];
    char version[10];
    char host[100];
    char content_type[100];
    char user_agent[200];
    char accept[100];
    int content_length;
    char body[BUFFER_SIZE];
    RequestError error;

}HttpRequest;

//prototypes
HttpRequest parse_request_function(int client_fd);


#endif 