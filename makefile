CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2 -Iinclude
LIBS = -lssl -lcrypto

server: src/main.c src/server.c src/request.c src/respond.c src/httpstatus.c
	gcc $(CFLAGS) -o build/server src/main.c src/server.c src/request.c src/respond.c src/httpstatus.c -lssl -lcrypto

clean:
	rm -rf build/server