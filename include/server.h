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

#define PORT 8080
#define BACKLOG 10
#define BUFFER_SIZE 1024

void server_function();

#endif   