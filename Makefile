CC = gcc

all:
	$(CC) http_server.c main.c routes.c -o server
