#include "server.h"

int main(void) {
    SSL_CTX *ctx = create_ssl_context();
    server_function(ctx);
    SSL_CTX_free(ctx);
    return 0;
}
