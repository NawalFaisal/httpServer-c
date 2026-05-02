#include "server.h"
#include "request.h"

//request needs to do these 4 things 
// read bytes from client
//parse the first line — method, path, version
//validate — is method valid? is path safe?
// parse the headers — Host, Content-Type etc

HttpRequest parse_request_function(int client_fd){
    //init struct
    HttpRequest req;
    //allocate memory to req and zero it out 
    memset(&req, 0, sizeof(req));

    // allocate memeory to buffer
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE) ;
    //now read the bytes in the buffer
    int read_bytes = read(client_fd, buffer, BUFFER_SIZE - 1); // we need to leave the last slot empty for null terminator
    //error check 
    if(read_bytes == -1){
        perror("unable to read request");
        // use the status error codes in struct
        req.error = REQUEST_BAD;
        return req; 
    }


    //parse the first line
    if (sscanf(buffer, "%9s %99s %9s", req.method, req.path, req.version) != 3){
        perror("Cannot parse first line");
        req.error = REQUEST_BAD;
        return req;
    }

    //validate methods
    if(strcmp(req.method, "GET") != 0 && 
      strcmp(req.method, "PUT")  != 0 &&
      strcmp(req.method, "POST") != 0 &&
      strcmp(req.method, "DELETE") != 0 ){
      req.error = REQUEST_METHOD_NOT_ALLOWED;
      return req;
      }

    //validate path
    if(strstr(req.path, "..") != NULL){
        req.error = REQUEST_FORBIDDEN;
        return req;
    }



    //parse headers
    //use strok_r
    char *saveptr;  //use this to track position

    char *line = strtok_r(buffer, "\r\n", &saveptr);

    while (line != NULL) {
    if (strncmp(line, "Host:", 5) == 0) {
        strncpy(req.host, line + 6, sizeof(req.host) - 1);
    } else if (strncmp(line, "Content-Type:", 13) == 0) {
        strncpy(req.content_type, line + 14, sizeof(req.content_type) - 1);
    } else if (strncmp(line, "User-Agent:", 11) == 0) {
        strncpy(req.user_agent, line + 12, sizeof(req.user_agent) - 1);
    } else if (strncmp(line, "Content-Length:", 15) == 0) {
        req.content_length = atoi(line + 16);
    }
    line = strtok_r(NULL, "\r\n", &saveptr);
}



    req.error = REQUEST_OK;
    return req;




}




