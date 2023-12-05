#ifndef ROUTES_H
#define ROUTES_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "http_server.h"

typedef struct ROUTES_Route{
  char* key;
  HTTP_Template* value;
  struct ROUTES_Route* left;
  struct ROUTES_Route* right;
}ROUTES_Route;

ROUTES_Route* ROUTES_InitRoute(char* key, HTTP_Template* value);
ROUTES_Route* ROUTES_AddRoute(ROUTES_Route* root, char* key, HTTP_Template* value);
ROUTES_Route* ROUTES_SearchRoute(ROUTES_Route* root, char* key);
void ROUTES_PrintRoutesInOrder(ROUTES_Route* root);

#endif
