#include "server.h"

void server_function() {

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

        char buffer [BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);

        int bytes_read = read(client_fd, BUFFER_SIZE - 1);
        if (bytes_read == -1){
            perror("Cannot read buffer")
            exit(1);
        }
        
    printf("request recieved\n")
    close(client_fd);
    exit(0); 

    } else {
        close(client_fd);
    }
}