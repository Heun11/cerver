#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#include "routes.h"
#include "http_server.h"

#define PORT 3030

int main(int argc, char *argv[])
{
  // init routes
  ROUTES_Route* root = ROUTES_InitRoute("/", HTTP_RenderTemplate("index.html"));
  ROUTES_AddRoute(root, "/about", HTTP_RenderTemplate("about.html"));
  ROUTES_AddRoute(root, "/404", HTTP_RenderTemplate("404.html"));
  ROUTES_PrintRoutesInOrder(root);

  // socket
  HTTP_Server server;
  HTTP_InitServer(&server, PORT);
  printf("running server on http://127.0.0.1:%d/\n\n", PORT);

  int client_socket;

  while(1){
    client_socket = accept(server.socket, NULL, NULL);

    char client_msg[4096] = "";
    read(client_socket, client_msg, 4095);
		// printf("%s\n", client_msg);

		// parsing client socket header to get HTTP method, route
		char *method = "";
		char *urlRoute = "";
		char *client_http_header = strtok(client_msg, "\n");
		printf("\n\t%s\n", client_http_header);
		char *header_token = strtok(client_http_header, " ");
	
		int header_parse_counter = 0;
		while (header_token != NULL) {
			switch (header_parse_counter) {
				case 0:
					method = header_token;
				case 1:
					urlRoute = header_token;
			}
			header_token = strtok(NULL, " ");
			header_parse_counter++;
		}

		printf("The method is %s\n", method);
		printf("The route is %s\n", urlRoute);

    char template[100] = "";

    ROUTES_Route* branch = ROUTES_SearchRoute(root, urlRoute);
    if(!branch){
      free(branch);
      branch = ROUTES_SearchRoute(root, "/404");
    }

    send(client_socket, branch->value->data, branch->value->size, 0);
    close(client_socket);
  }
    
  return 0;
}
