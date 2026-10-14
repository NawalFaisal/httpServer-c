#ifndef HTTP_STATUS_H
#define HTTP_STATUS_H

//enum of methods mapped to their number
//at runtime these methods get replaces with their error numbers
typedef enum {
    HTTP_OK = 200,
    HTTP_BAD_REQUEST = 400,
    HTTP_UNAUTHORIZED = 401,
    HTTP_FORBIDDEN = 403,
    HTTP_NOT_FOUND = 404,
    HTTP_METHOD_NOT_ALLOWED = 405,
    HTTP_PAYLOAD_TOO_LARGE = 413,
    HTTP_INTERNAL_SERVER_ERROR = 500,
    HTTP_SERVICE_UNAVAILABLE = 503
} HttpStatus;

const char *http_status_text(HttpStatus status);

#endif //HTTP_STATUS_H