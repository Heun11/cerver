#include "http_server.h"
#include <stdlib.h>
#include <string.h>

void HTTP_InitServer(HTTP_Server* http_server, int port)
{
  // socket init
  http_server->socket = socket(AF_INET, SOCK_STREAM, 0);
  http_server->port = port;

  if (setsockopt(http_server->socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0){
    printf("setsockopt(SO_REUSEADDR) failed");
    exit(0);
  }

  http_server->staticDir = "static/";
  http_server->templateDir = "templates/";
  http_server->resourceDir = "resources/";
  http_server->scriptDir = "scripts/";
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

void HTTP_AddDir(HTTP_Server* http_server, char* dir_type, char* dir_name)
{
  if(strcmp(dir_type, "static")==0){
    http_server->staticDir = dir_name;
  }
  else if(strcmp(dir_type, "template")==0){
    http_server->templateDir = dir_name;
  }
  else if(strcmp(dir_type, "resource")==0){
    http_server->resourceDir = dir_name;
  }
  else if(strcmp(dir_type, "script")==0){
    http_server->scriptDir = dir_name;
  }
}

char* HTTP_GetContentType(char* filename)
{
  char buff1[MAXFILENAME];
  char buff2[MAXFILENAME];
  char* token;

  strcpy(buff1, filename);
  token = strtok(buff1, ".");
  while( token != NULL ) {
    strcpy(buff2, token);
    token = strtok(NULL, ".");
  }
  // printf("%s\n", buff2);

  if(strcmp("html", buff2)==0){
    strcpy(buff1, "Content-Type: text/html \r\n");
  }
  else if(strcmp("css", buff2)==0){
    strcpy(buff1, "Content-Type: text/css \r\n");
  }
  else if(strcmp("js", buff2)==0){
    strcpy(buff1, "Content-Type: text/javascript \r\n");
  }
  else if(strcmp("jpg", buff2)==0 || strcmp("jpeg", buff2)==0){
    strcpy(buff1, "Content-Type: image/jpeg \r\n");
  }
  else if(strcmp("png", buff2)==0){
    strcpy(buff1, "Content-Type: image/png \r\n");
  }
  else if(strcmp("gif", buff2)==0){
    strcpy(buff1, "Content-Type: image/gif \r\n");
  }
  else{
    strcpy (buff1, "Content-Type: application/octet-stream \r\n");
  }

  char* header_buff = buff1;
  
  return header_buff;
}

HTTP_Template* HTTP_RenderTemplate(char* dir_name, char* filename)
{
  FILE* html_data;
  int sz;
  char* buff;
  char ch;
  HTTP_Template* tmp;
  int i;
  char fnBuff[MAXFILENAME];
  char resData[1024];

  tmp = (HTTP_Template*)malloc(sizeof(HTTP_Template));
  strcpy(fnBuff, dir_name);
  strcat(fnBuff, filename);
  if(access(fnBuff, F_OK)==0){
    html_data = fopen(fnBuff, "r");
  }else {
    printf("error - file dont exist\n");
    exit(0);
  }

  fseek(html_data, 0L, SEEK_END);
  sz = ftell(html_data);
  fseek(html_data, 0L, SEEK_SET);
  
  buff = malloc((sz+1)*sizeof(char));
  
  i = 0;
  while((ch=fgetc(html_data))!=EOF){
    buff[i] = ch;
    i++;
  }
  
  char buff1[MAXFILENAME];
  char buff2[MAXFILENAME];
  strcpy(buff1, filename);
  char* token = strtok(buff1, ".");
  while( token != NULL ) {
    strcpy(buff2, token);
    token = strtok(NULL, ".");
  }

  if((strcmp("html", buff2)==0)||(strcmp("css", buff2)==0)||(strcmp("js", buff2)==0)||
  (strcmp("txt", buff2)==0)||(strcmp("plain", buff2)==0)||(strcmp("csv", buff2)==0)){
    buff[i]='\0';
  }
  else{
    buff[i]=EOF;
  }

  strcpy(resData, "HTTP/1.1 200 OK\r\n");
  strcat(resData, HTTP_GetContentType(filename));
  strcat(resData, "\n");

  tmp->size = (sz+strlen(resData))*sizeof(char);
  tmp->data = malloc(tmp->size);
  strcpy(tmp->data, resData);
  strcat(tmp->data, buff);

  fclose(html_data);
  free(buff);
  return tmp;
}

