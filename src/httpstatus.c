#include "httpstatus.h"
#include <stddef.h>


//this unamed struct that i dont seem to be using anywhere is static to this file and its a constant
//wait my arrray is the name of it ?
static const struct {
    HttpStatus status;
    const char* text;
} 

//this is an array so its a key value look up 
//i need this is source code so it allcoates mem to array at runtime
status_table[] = {
    {HTTP_OK, "OK"},
    {HTTP_BAD_REQUEST, "Bad Request"},
    {HTTP_UNAUTHORIZED, "Unauthorized"},
    {HTTP_FORBIDDEN, "Forbidden"},
    {HTTP_NOT_FOUND, "Not Found"},
    {HTTP_METHOD_NOT_ALLOWED, "Method Not Allowed"},
    {HTTP_PAYLOAD_TOO_LARGE, "Payload Too Large"},
    {HTTP_INTERNAL_SERVER_ERROR, "Internal Server Error"},
    {HTTP_SERVICE_UNAVAILABLE, "Service Unavailable"},
};

//http status text is a pointer to a char constant
const char *http_status_text(HttpStatus status) {
    size_t n = sizeof(status_table) / sizeof(status_table[0]);  //compute how many elemnts in array so 
    for (size_t i = 0; i < n; i++) { // you can stop right beofre you hit garbge in your for loop
        if (status_table[i].status == status) {
            return status_table[i].text;
        }
    }
    return "Internal Server Error";
}