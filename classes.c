#include "classes.h"
#include <stdlib.h>
#include <string.h>

#define A 2

void hierarchy_create(struct oo_hierarchy *self) {
  //Créer un noeud racine avec comme nom objet
  struct oo_class *node = calloc(1,sizeof(struct oo_class));
  node->name = "Object";
  node->parent = NULL;
  node->capacity = 1;
  node->children = calloc(node->capacity,sizeof(struct oo_class));
  self->root = node;
}


// fonction rajouté
void class_destroy(struct oo_class *self){
  if (self->children == NULL){
    free(self);
    return;
  }
  for (size_t i=0 ; i<self->size ; ++i){
    class_destroy(self->children[i]);
  }
}
// fonction rajouté


void hierarchy_destroy(struct oo_hierarchy *self) {
  class_destroy(self->root);
  self->root = NULL;
}


// fonction rajouté
size_t child_count(struct oo_class *self){
  size_t cpt=0;
  if(self->children == NULL){
    return cpt;
  }

  for (size_t i=0 ; i< self->size ; ++i){
    cpt = cpt + 1 + child_count(self->children[i]);
  }

  return cpt;
}
// fonction rajouté

size_t hierarchy_count_classes(const struct oo_hierarchy *self) {
  return 1 + child_count(self->root);
}

// fonction rajouté
bool has_child_named(struct oo_class *self, const char *name){
  if(self->children == NULL){
    return false;
  }

  for (size_t i=0 ; i< self->size ; ++i){
    if(strcmp(self->children[i]->name, &name) == 0 || has_child_named(self->children[i], &name)){
      return true;
    }
  }

  return false;
}
// fonction rajouté

bool hierarchy_has_class(const struct oo_hierarchy *self, const char *name) {
  return has_child_named(self->root, &name);
}

// fonction rajouté
void class_add(struct oo_class *self, const char *child) {
  if (self->size == self->capacity) {
    self->capacity *= A;
    struct oo_class **children = calloc(self->capacity, sizeof(struct oo_class));
    memcpy(children, self->children, self->size * sizeof(struct oo_class));
    free(self->children);
    self->children = children;
  }
  self->children[self->size]->name = child;
  self->children[self->size]->parent= self;
  self->size += 1;
}

bool class_add_child(struct oo_class *self, const char *parent, const char *child){
  
  if(strcmp(self->name,&parent)==0){
    class_add(self,child);
    return true;
  }

  for (size_t i=0 ; i< self->size ; ++i){

    if(class_add_child(self->children[i] , &parent, &child)){
      return true;
    }
  }
  return false;
}
// fonction rajouté

bool hierarchy_add_child(struct oo_hierarchy *self, const char *parent, const char *child) {
  return class_add_child(self->root,parent,child);
}
bool class_is_child_of(const struct oo_class *self, const char *parent){
  if(self->parent == NULL){
    return false;
  }
  if(strcmp(self->parent->name,&parent)==0){
    return true;
  }

  return class_is_child_of(self->parent,parent);
}
bool class_is_child_of_allTree(const struct oo_class *self, const char *parent,const char *child){
  if(strcmp(self->name,&child)==0){
    return class_is_child_of(self,parent);
  }

  for (size_t i=0 ; i< self->size ; ++i){
    if(class_is_child_of_allTree(self->children[i],parent,child)){
      return true;
    }
  }
  return false;
}
bool hierarchy_is_child_of(const struct oo_hierarchy *self, const char *parent, const char *child) {
  return class_is_child_of_allTree(self->root,parent,child);
}
bool class_delete(struct oo_class *self, const char *name){
  if(strcmp(self->name,name)==0){
    free(self);
    return true;
  }
  for(size_t i=0; i<self->size ; ++i){
    if(class_delete(self->children[i]->name, name)){
      return true;
    }
  }
  return false;
}
bool hierarchy_delete(struct oo_hierarchy *self, const char *name) {
  return class_delete(self->root,name);
}
bool class_rename(struct oo_class *self, const char *old_name, const char *new_name){
  if(strcmp(self->name,old_name)==0){
    self->name = new_name;
    return true;
  }
  for(size_t i=0; i<self->size ; ++i){
    if(class_rename(self->children[i]->name, old_name , new_name)){
      return true;
    }
  }
  return false;
}
bool hierarchy_rename(struct oo_hierarchy *self, const char *old_name, const char *new_name){
  return class_rename(self->root,old_name,new_name);
}

bool hierarchy_move_as_child_of(struct oo_hierarchy *self, const char *name, const char *parent){
  return false;//hierarchy_move();
}
bool class_add_path(struct oo_class *self,const char *path){
  if(self==NULL){
    return false;
  }
  const char delim[2] = "/";
  char *token;
  token = strtok(path, delim);

  if (strcmp(token,"Object")==0){
    token = strtok(NULL,delim);
  }

  struct oo_class curr = *self;

  while (token!=NULL){
    class_add(self, token);
    curr=*self->children[0];
    token = strtok(NULL,delim);
  }

  return true;
}
bool hierarchy_add_path(struct oo_hierarchy *self, const char *path) {
  return class_add_path(self->root,path);
}

bool class_add_path_as_child_of(struct oo_class *self, const char *path, const char *parent){
  if(strcmp(self->name,parent)==0){
    const char delim[2] = "/";
    char *token;
    token = strtok(path, delim);
    while (token!=NULL){
      if(has_child_named(self,token)){
        return false;
      }
      token = strtok(NULL,delim);
    }
    
    return class_add_path(self,path);
  }
  for(size_t i=0; i<self->size ; ++i){
    if(class_add_path_as_child_of(self->children[i],path,parent)){
      return true;
    }
  }
  return false;
}
bool hierarchy_add_path_as_child_of(struct oo_hierarchy *self, const char *path, const char *parent){
  return class_add_path_as_child_of(self->root, path, parent);
}

char *class_get_path_to(const struct oo_class *self, const char *name){
  char* res = NULL;
  char* tmp = NULL;

  size_t tailleRes=0;
  size_t tailleNom=0;

  if(strcmp(self->name,name)==0){
    struct oo_class *curr = self;
    do{
      tailleRes = strlen(res);
      tailleNom = strlen(curr->name);
      res = malloc(tailleRes + tailleNom + 1);
      tmp = malloc(tailleRes);
      strncpy(tmp,res,tailleRes);
      strncpy(res,"/",1);
      strncpy(res+1,curr->name,tailleNom);
      strcpy(res + 1 + tailleNom,tmp);
      curr = curr->parent;
    }while(curr->parent !=NULL);
    free(tmp);
    return res;
  }

  for(size_t i=0; i<self->size ; ++i){
    tmp = class_get_path_to(self->children[i], name);
    if(tmp!=NULL){
      return tmp;
    }
  }

  return res;
}
char *hierarchy_get_path_to(const struct oo_hierarchy *self, const char *name) {
  return class_get_path_to(self->root,name);
}

void hierarchy_sort(struct oo_hierarchy *self) {

}
void class_print(const struct oo_class *self, FILE *out){
  printf("%c\n",class_get_path_to(self,self->name));
  for(size_t i=0; i<self->size ; ++i){
    class_print(self->children[i],out);
  }
}
void hierarchy_print(const struct oo_hierarchy *self, FILE *out) {
  class_print(self->root,out);
}

bool hierarchy_move(struct oo_hierarchy *self, const char *origin, const char *destination) {
  return false;
}
