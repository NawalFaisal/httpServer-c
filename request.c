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



    return req;



}




