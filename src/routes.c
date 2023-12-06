#include "routes.h"

ROUTES_Route* ROUTES_InitRoute(char* key, HTTP_Template* value)
{
  ROUTES_Route* root;
  root = (ROUTES_Route*)malloc(sizeof(ROUTES_Route));
  root->key = key;
  root->value = value;

  root->left = NULL;
  root->right = NULL;

  return root;
}
  
void ROUTES_PrintRoutesInOrder(ROUTES_Route* root)
{
  if(root){
    ROUTES_PrintRoutesInOrder(root->left);
    printf("%s \n", root->key);
    ROUTES_PrintRoutesInOrder(root->right);
  }
}

ROUTES_Route* ROUTES_AddRoute(ROUTES_Route* root, char* key, HTTP_Template* value)
{
  if(!root){
    return ROUTES_InitRoute(key, value);
  }

  if(strcmp(key, root->key)==0){
		printf("WARNING:\n");
		printf("\tA Route For \"%s\" Already Exists\n", key);
	}
  else if(strcmp(key, root->key)>0){
		root->right = ROUTES_AddRoute(root->right, key, value);
	}
  else{
		root->left = ROUTES_AddRoute(root->left, key, value);
	}

  return root;
}

ROUTES_Route* ROUTES_SearchRoute(ROUTES_Route* root, char* key)
{
  if(!root){
    return NULL;
  }

  if(strcmp(key, root->key)==0){
    return root;
  }
  else if(strcmp(key, root->key)>0){
    return ROUTES_SearchRoute(root->right, key);
  }
  else{
    return ROUTES_SearchRoute(root->left, key);
  }
}


