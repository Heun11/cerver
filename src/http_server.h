#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>

#define MAXFILENAME 200
#define HEADERSIZE 1024
#define MAXLEN 100000

typedef struct{
  int socket;
  int port;
  char* staticDir;
  char* templateDir;
  char* resourceDir;
  char* scriptDir;
}HTTP_Server;

typedef struct{
  char* data;
  int size;
}HTTP_Template;

typedef struct{
  char* data;
  int size;
}HTTP_FileContent;

void HTTP_InitServer(HTTP_Server* http_server, int port);
void HTTP_AddDir(HTTP_Server* http_server, char* dir_type, char* dir_name);
char* HTTP_GetContentType(char* filename);
HTTP_FileContent* HTTP_GetContentOfFile(char* dir_name, char* filename, char* filetype);
HTTP_Template* HTTP_RenderTemplate(char* dir_name, char* filename);

#endif
