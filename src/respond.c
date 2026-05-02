#include "respond.h"

char *content_type_function(char *path){
   if (strstr(path, ".html")){
    return("text/html");
    return("text/plain");
   }
}


void error_response_function(int client_fd, int status_code, *message){
    
}
