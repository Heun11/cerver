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
    // socket
  HTTP_Server server;
  HTTP_InitServer(&server, PORT);
  printf("running server on http://127.0.0.1:%d/\n\n", PORT);

  // adding folders
  HTTP_AddDir(&server, "statc", "static/");
  HTTP_AddDir(&server, "template", "templates/");
  HTTP_AddDir(&server, "resource", "resources/");
  HTTP_AddDir(&server, "script", "js/");

  // html routes
  ROUTES_Route* root = ROUTES_InitRoute("/", HTTP_RenderTemplate(server.templateDir, "index.html"));
  ROUTES_AddRoute(root, "/about", HTTP_RenderTemplate(server.templateDir, "about.html"));
  ROUTES_AddRoute(root, "/404", HTTP_RenderTemplate(server.templateDir, "404.html"));
  // css routes
  ROUTES_AddRoute(root, "/style.css", HTTP_RenderTemplate(server.staticDir, "style.css"));
  // js routes
  ROUTES_AddRoute(root, "/script.js", HTTP_RenderTemplate(server.scriptDir, "script.js"));

  printf("\n%s\n", ROUTES_SearchRoute(root, "/script.js")->value->data);
  // show all routes
  ROUTES_PrintRoutesInOrder(root);

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
		printf("\n%s\n", client_http_header);
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
