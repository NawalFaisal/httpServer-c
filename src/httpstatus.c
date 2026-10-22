#include "httpstatus.h"
#include <stddef.h>

static const struct {
    HttpStatus status;
    const char* text;
} status_table[] = {
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