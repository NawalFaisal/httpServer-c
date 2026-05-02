CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2 -Iinclude

server: src/main.c src/server.c src/request.c src/respond.c
	$(CC) $(CFLAGS) -o build/server src/main.c src/server.c src/request.c src/respond.c

clean:
	rm -rf build/server