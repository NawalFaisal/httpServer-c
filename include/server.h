#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>       
#include <string.h>        
#include <unistd.h>        
#include <sys/socket.h>    
#include <sys/types.h>    
#include <netinet/in.h>    
#include <arpa/inet.h>     
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT 8443
#define BACKLOG 10
#define BUFFER_SIZE 1024
#define CERT_FILE "certs/cert.pem"
#define KEY_FILE  "certs/key.pem"

SSL_CTX *create_ssl_context(void);
void server_function(SSL_CTX *ctx);

#endif   