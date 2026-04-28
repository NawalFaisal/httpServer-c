CC = gcc
CFLAGS = -Wall -Wextra

server: main.c server.c request.c
	$(CC) $(CFLAGS) -o server main.c server.c request.c

clean:
	rm -f server