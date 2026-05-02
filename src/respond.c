#include "respond.h"

char *content_type_function(char *path) {
    if (strstr(path, ".html")) return "text/html";
    return "text/plain";
}

void error_response_function(int client_fd, RequestError error) {
    int status_code;
    char *message;

    switch(error) {
        case REQUEST_BAD:
            status_code = 400;
            message = "Bad Request";
            break;
        case REQUEST_UNAUTHORIZED:
            status_code = 401;
            message = "Unauthorized";
            break;
        case REQUEST_FORBIDDEN:
            status_code = 403;
            message = "Forbidden";
            break;
        case REQUEST_NOT_FOUND:
            status_code = 404;
            message = "Not Found";
            break;
        case REQUEST_METHOD_NOT_ALLOWED:
            status_code = 405;
            message = "Method Not Allowed";
            break;
        case REQUEST_PAYLOAD_TOO_LARGE:
            status_code = 413;
            message = "Payload Too Large";
            break;
        case REQUEST_SERVICE_UNAVAILABLE:
            status_code = 503;
            message = "Service Unavailable";
            break;
        default:
            status_code = 500;
            message = "Internal Server Error";
            break;
    }

    // response string
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    snprintf(buffer, sizeof(buffer),
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><body><h1>%d %s</h1></body></html>",
        status_code, message,
        status_code, message);

    write(client_fd, buffer, strlen(buffer));
}

void send_response_function(int client_fd, HttpRequest *req) {
    char file_path[200];
    if (strcmp(req->path, "/") == 0) {
        snprintf(file_path, sizeof(file_path), "static/index.html");
    } else {
        snprintf(file_path, sizeof(file_path), "static%s", req->path);
    }

    // open 
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        error_response_function(client_fd, REQUEST_NOT_FOUND);
        return;
    }

    // get size of file
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // read file into buffer
    char *body = malloc(file_size + 1);
    fread(body, 1, file_size, file);
    body[file_size] = '\0';
    fclose(file);

    // get content type
    char *content_type = content_type_function(file_path);

    // build and send headers
    char headers[BUFFER_SIZE];
    snprintf(headers, sizeof(headers),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "\r\n",
        content_type, file_size);

    write(client_fd, headers, strlen(headers));
    write(client_fd, body, file_size);
    free(body);
}