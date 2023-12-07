#include "http_server.h"
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
  free(token);

  char* header_buff = (char*)malloc((1+strlen(buff1))*sizeof(char));
  strcpy(header_buff, buff1);
  
  return header_buff;
}

HTTP_FileContent* HTTP_GetContentOfFile(char* dir_name, char* filename, char* filetype)
{
  FILE* fp;
  int fs;
  char fnBuff[MAXFILENAME];
  char* buff;
  int i;
  char ch;
  HTTP_FileContent* fcp;

  strcpy(fnBuff, dir_name);
  strcat(fnBuff, filename);
  if(access(fnBuff, F_OK)==0){
    if(strcmp(filetype, "Content-Type: text/html \r\n")==0 || strcmp(filetype, "Content-Type: text/css \r\n")==0 
    || strcmp(filetype, "Content-Type: text/javascript \r\n")==0 || strcmp(filetype, "Content-Type: text/plain \r\n")==0 
    || strcmp(filetype, "Content-Type: text/csv \r\n")==0 || strcmp(filetype, "Content-Type: text/xml \r\n")==0){
      // read text   
      fp = fopen(fnBuff, "r");
      fseek(fp, 0L, SEEK_END);
      fs = ftell(fp);
      fseek(fp, 0L, SEEK_SET);
      fs++;

      buff = (char*)malloc((fs)*sizeof(char));
      i = 0;
      while((ch=fgetc(fp))!=EOF){
        buff[i] = ch;
        i++;
      }
      buff[i]='\0';
    }
    else{
      // read binary
      fp = fopen(fnBuff, "rb");
      fseek(fp, 0L, SEEK_END);
      fs = ftell(fp);
      fseek(fp, 0L, SEEK_SET);
      fs++;

      buff = (char*)malloc((fs)*sizeof(char));
      fread(buff, fs, 1, fp);
    }
    fclose(fp);
    fcp = (HTTP_FileContent*)malloc(sizeof(HTTP_FileContent));
    fcp->data = buff;
    fcp->size = fs;
  }
  else{
    printf("Error opening file\n");
    exit(0);
  }
  
  return fcp;
}

HTTP_Template* HTTP_RenderTemplate(char* dir_name, char* filename)
{
  HTTP_Template* tmp;
  char resData[HEADERSIZE];

  tmp = (HTTP_Template*)malloc(sizeof(HTTP_Template));
  HTTP_FileContent* fcp = HTTP_GetContentOfFile(dir_name, filename, HTTP_GetContentType(filename));

  strcpy(resData, "HTTP/1.1 200 OK\r\n");
  strcat(resData, HTTP_GetContentType(filename));
  char content_size_buff[64];
  sprintf(content_size_buff, "Content-Length: %d\n", fcp->size);
  strcat(resData, content_size_buff);
  strcat(resData, "Accept-Ranges: bytes\n");
  strcat(resData, "Connection: close\n");
  strcat(resData, "\n");

  tmp->size = (fcp->size+strlen(resData))*sizeof(char);
  tmp->data = malloc(tmp->size);
  strcpy(tmp->data, resData);
  memcpy(tmp->data+strlen(resData), fcp->data, fcp->size);
  
  free(fcp->data);
  free(fcp);

  // printf("%s\n", tmp->data);
  return tmp;
}

