#include "server.h"
#include "request.h"
// #include "httpstatus.h"
#include <openssl/ssl.h>



//also having one function handle numberous things seems to be kinda not very solid principles 
// i should seperate the parsing,validating and error checking into three seperate fucntions or 
//i should take my request and seperate it into start, headers and body and all three will have their own parsing validating and error handling
//not sure which one is more effeint but will come back as of right now im satisified with this

HttpRequest parse_request_function(SSL *ssl){
    HttpRequest req;
    //allocate memory to req and zero it out 
    memset(&req, 0, sizeof(req));

    // allocate memeory to buffer
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE) ;
    //now read the bytes in the buffer
    int read_bytes = SSL_read(ssl, buffer, BUFFER_SIZE - 1); // we need to leave the last slot empty for null terminator
    //error check 
    if(read_bytes <= 0){
        ERR_print_errors_fp(stderr);
        // use the status error codes in struct
        // HTTP_BAD_REQUEST;  //throws a statement with no affect error not sure why i made this design decision
        return req; 
    }


    //parse the first line
     // post /http /1.1 
     //ngl this seems weak but we prevail
    if (sscanf(buffer, "%9s %99s %9s", req.method, req.path, req.version) != 3){
        perror("Cannot parse first line");
        // HTTP_BAD_REQUEST;
        return req;
    }


    //validate methods
    // while its not a magic number i feel like taking the string i our req method we parsed from the buffer and comaping it with the string methods seems weak
     // is this case senstive what is its get instead of GET 
     //not sure if zero can be cosnisdered a magic number 

    if(strcmp(req.method, "GET") != 0 && 
      strcmp(req.method, "PUT")  != 0 &&
      strcmp(req.method, "POST") != 0 &&
      strcmp(req.method, "DELETE") != 0 ){
    //   HTTP_METHOD_NOT_ALLOWED;
    //dont use htttps status actually handle the error 
      return req;
      }


    //parse headers
    //use strok_r instead of strok for thread saftey when we move away from forking.
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

    // HTTP_OK;
    return req;




}




