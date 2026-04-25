CC = gcc
CFLAGS = -Wall -Wextra

server: main.c server.c
	$(CC) $(CFLAGS) -o server main.c server.c

clean:
	rm -f server