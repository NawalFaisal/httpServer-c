#include "respond.h"
#include "httpstatus.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "request.h"


//fucntion declratins
const char *http_status_text(HttpStatus status);

// function to determine what content type it returns
char *content_type_function(char *path) {
    if (strstr(path, ".html")) return "text/html";
    return "text/plain";
}


void error_response_function(SSL *ssl, HttpStatus status) {
    const char *text = http_status_text(status);
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    snprintf(buffer, sizeof(buffer),
        "HTTP/1.1 %d %s\r\n"  
        "Content-Type: text/html\r\n" 
        "\r\n"
        "<html><body><h1>%d %s</h1></body></html>",
         status, text, status, text);  //call my http status text message it will print the array of both status and text mapped to the status that passsed as an argument
   
    if (SSL_write(ssl, buffer, strlen(buffer)) <= 0) {
        ERR_print_errors_fp(stderr);
    }
}


void send_response_function(SSL *ssl, HttpRequest *req) {
    char file_path[200];
    if (strcmp(req->path, "/") == 0) {
        snprintf(file_path, sizeof(file_path), "static/index.html");
    } else {
        snprintf(file_path, sizeof(file_path), "static%s", req->path);
    }

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        error_response_function(ssl, HTTP_NOT_FOUND);
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *body = malloc(file_size + 1);
    if (fread(body, 1, file_size, file) == 0) {
        perror("Cannot read file");
        free(body);
        fclose(file);
        return;
    }

    body[file_size] = '\0';
    fclose(file);

    char *content_type = content_type_function(file_path);

    char headers[BUFFER_SIZE];
    snprintf(headers, sizeof(headers),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "\r\n",
        content_type, file_size);

    if (SSL_write(ssl, headers, strlen(headers)) <= 0) {
        fprintf(stderr, "Cannot write headers\n");
        ERR_print_errors_fp(stderr);
    }
    if (SSL_write(ssl, body, file_size) <= 0) {
        fprintf(stderr, "Cannot write body\n");
        ERR_print_errors_fp(stderr);
    }
    free(body);
}