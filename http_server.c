#include "http_server.h"
#include <stdlib.h>

void HTTP_InitServer(HTTP_Server* http_server, int port)
{
  // socket init
  http_server->socket = socket(AF_INET, SOCK_STREAM, 0);
  http_server->port = port;
  // adress
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(http_server->port);
  server_address.sin_addr.s_addr = INADDR_ANY;

  if(bind(http_server->socket, (struct sockaddr*) &server_address, sizeof(server_address))){
    printf("Unable to bind local address\n");
    exit(0);
  }

  listen(http_server->socket, 5);
}


HTTP_Template* HTTP_RenderTemplate(char* filename)
{
  FILE* html_data;
  int sz;
  char* buff;
  char ch;
  HTTP_Template* tmp;
  int i;

  tmp = (HTTP_Template*)malloc(sizeof(HTTP_Template));
  html_data = fopen(filename, "r");

  fseek(html_data, 0L, SEEK_END);
  sz = ftell(html_data);
  fseek(html_data, 0L, SEEK_SET);
  
  buff = malloc((sz+1)*sizeof(char));
  
  i = 0;
  while((ch=fgetc(html_data))!=EOF){
    buff[i] = ch;
    i++;
  }

  tmp->size = sz*1024*sizeof(char);
  tmp->data = malloc(tmp->size);
  strncpy (tmp->data, "HTTP/1.1 200 OK\r\n\n", 1024);
  strcat(tmp->data, buff);

  fclose(html_data);
  free(buff);
  return tmp;
}

