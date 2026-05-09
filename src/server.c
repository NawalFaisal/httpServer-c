#include "server.h"
#include "request.h"
#include "respond.h"


//sll function create context we need to bundle information like or key and cert so they know hey this is safe and has abadge
//this is just a factory function. we call it it gives us the new ssl context and we need to store it in varibale 
SSL_CTX *create_ssl_context(void){

    // use the server method 
    const SSL_METHOD *method = TLS_server_method();
    // ctx stores our new ssl context.
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (ctx == NULL) {
    perror("Cannot create SSL context");
    ERR_print_errors_fp(stderr);
    exit(1);

}
    // force recent versions
    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);

    // load certificate
    if (SSL_CTX_use_certificate_file(ctx, CERT_FILE, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    // load private key
    if (SSL_CTX_use_PrivateKey_file(ctx, KEY_FILE, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    // verify key matches certificate
    if (!SSL_CTX_check_private_key(ctx)) {
        perror("Private key does not match certificate");
        exit(1);
    }

    return ctx;
}


void server_function(SSL_CTX *ctx) {

    int serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_fd == -1) {
        perror("Cannot create socket");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serv_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("Cannot bind");
        exit(1);
    }

    if (listen(serv_fd, BACKLOG) == -1) {
        perror("Cannot listen");
        exit(1);
    }

    while(1){
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(serv_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("Cannot accept");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Cannot fork");
        exit(1);
    }

    if (pid == 0) {
        close(serv_fd);
        //get and instance from our factory
        SSL *ssl = SSL_new(ctx);
        //attach to our client fd
       SSL_set_fd(ssl, client_fd);

        //TLS handshake
    if (SSL_accept(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        close(client_fd);
        exit(1);
    }
    
        //call request function and store it in the struct
        HttpRequest req = parse_request_function(ssl);

        if (req.error != REQUEST_OK) {
        error_response_function(ssl, req.error);
        } else {
            send_response_function(ssl, &req);
        }


        // cleanup
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client_fd);
        exit(0);
        
    } else {
        close(client_fd);
    }


}
}
