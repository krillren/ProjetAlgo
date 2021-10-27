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
  self->size += 1;
}

bool class_add_child(struct oo_class *self, const char *parent, const char *child){
  
  if(strcmp(self->name,&parent)==0){
    class_add(self,child);
    return true;
  }

  if(self->children == NULL){
    return false;
  }

  for (size_t i=0 ; i< self->size ; ++i){

    if(strcmp(self->children[i]->name, &parent)==0){
      class_add(self->children[i],child);
      return true;
    }
    class_add_child(self->children[i] , &parent, &child);

  }
  return false;
}
// fonction rajouté

bool hierarchy_add_child(struct oo_hierarchy *self, const char *parent, const char *child) {
  return class_add_child(self->root,parent,child);
}

bool hierarchy_is_child_of(const struct oo_hierarchy *self, const char *parent, const char *child) {
  return false;
}

bool hierarchy_delete(struct oo_hierarchy *self, const char *name) {
  return false;
}

bool hierarchy_add_path(struct oo_hierarchy *self, const char *path) {
  return false;
}

char *hierarchy_get_path_to(const struct oo_hierarchy *self, const char *name) {
  return NULL;
}

void hierarchy_sort(struct oo_hierarchy *self) {

}

void hierarchy_print(const struct oo_hierarchy *self, FILE *out) {

}

bool hierarchy_move(struct oo_hierarchy *self, const char *origin, const char *destination) {

}
