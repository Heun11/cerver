#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

typedef struct{
  int socket;
  int port;
}HTTP_Server;

typedef struct{
  char* data;
  int size;
}HTTP_Template;

void HTTP_InitServer(HTTP_Server* http_server, int port);
HTTP_Template* HTTP_RenderTemplate(char* filename);

#endif
