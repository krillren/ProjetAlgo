#include "C:\DevC\Projet_algo_classe\classes.h"
#include <stdlib.h>
#include <string.h>



char * creerChaineNom(const char* nom) {
    char * tmpc = malloc(strlen(nom) + 1);
    if (tmpc != NULL) {
        strcpy(tmpc, nom);
    }
    return tmpc;
}

void libererChaineNom(char*nom) {
    free(nom);
}
char * strcatEN_MIEUX(char *res, char *nom,const char * delim) {
    char* tmp=NULL;
    size_t tailleRes=0;
    size_t tailleNom = 0;
    size_t tailleDelim = 0;

    if (res != NULL && strlen(res) >=1 ) {
        tailleRes = strlen(res);
        tmp = malloc(tailleRes + 1);
        if (tmp != NULL) {
            strcpy(tmp, res);
        }
    }
    else {
        tailleRes = 0;
    }

    if (nom != NULL) {
        tailleNom = strlen(nom);
        res = malloc(1 + tailleNom + tailleRes+1);

        if (res!=NULL) {
            if (delim != NULL && strlen(delim) >= 1) {
                tailleDelim = 1;
                strncpy(res, delim, 1);
            }
            
            strncpy(res + tailleDelim, nom, tailleNom);

            if ( tmp!=NULL) {
                strncpy(res + tailleDelim + tailleNom, tmp, tailleRes);
            }

            res[tailleDelim + tailleNom + tailleRes] = '\0';
        }

        free(tmp);
        return res;
    }
    

   
    free(tmp);
    return "Erreur_De_Nom";
}

struct oo_class* getChild(struct oo_class* self, char* name) {
    for (size_t i = 0; i < self->size; ++i) {
        if (self->children[i] != NULL) {
            if (strcmp(self->children[i]->name, name) == 0) {
                return self->children[i];
            }
        }
    }
    return NULL;
}

void hierarchy_create(struct oo_hierarchy *self) {
  //Créer un noeud racine avec comme nom objet
  struct oo_class *node = calloc(1,sizeof(struct oo_class));
  if (node != NULL) {
      node->name = creerChaineNom("Object");
      node->parent = NULL;
      node->capacity = 1;
      node->children = calloc(node->capacity, sizeof(struct oo_class*));
      self->root = node;
  }
}


// fonction rajouté
void class_destroy(struct oo_class *self){
  if (self->children == NULL){
    free(self);
    return;
  }
  for (size_t i=0 ; i<self->size ; ++i){
      if (self->children[i] != NULL) {
          class_destroy(self->children[i]);
      }
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
  if (self != NULL) {
      for (size_t i = 0; i < self->size; ++i) {
          if (self->children[i] != NULL) {
              cpt = cpt + 1 + child_count(self->children[i]);
          }

      }
  }
  return cpt;
}
// fonction rajouté

size_t hierarchy_count_classes(const struct oo_hierarchy *self) {
  if (self != NULL && self->root != NULL) {
    return 1 + child_count(self->root);
  }
  return 0;
}

// fonction rajouté
bool has_child_named(struct oo_class *self, const char *name){
  if(self->children == NULL){
    return false;
  }

  for (size_t i=0 ; i< self->size ; ++i){
      if (self->children[i] != NULL) {

          if (strcmp(self->children[i]->name, name) == 0 || has_child_named(self->children[i], name)) {
              return true;
          }
      }
  }

  return false;
}
// fonction rajouté

bool hierarchy_has_class(const struct oo_hierarchy *self, const char *name) {
  return has_child_named(self->root, name);
}

// fonction rajouté
void class_add(struct oo_class *self, const char *child) {
  if (self->size == self->capacity) {
    self->capacity *= 2;
    struct oo_class **children = calloc(self->capacity, sizeof(struct oo_class*));
    if (children != NULL) {
        memcpy(children, self->children, self->size * sizeof(struct oo_class*));
        free(self->children);
        self->children = children;
    }
  }

  if (self->children != NULL) {
      self->children[self->size] = calloc(1, sizeof(struct oo_class));
      if (self->children[self->size] != NULL) {
          self->children[self->size]->name = creerChaineNom(child);
          self->children[self->size]->parent = self;
          self->children[self->size]->capacity = 1;
          self->children[self->size]->children = calloc(1, sizeof(struct oo_class*));
          self->size += 1;
      }
  }
 
}

bool class_add_child(struct oo_class *self, const char *parent, const char *child){
    if (self != NULL) {
        if (strcmp(self->name, parent) == 0) {
            class_add(self, child);
            return true;
        }


        for (size_t i = 0; i < self->size; ++i) {
            if (self->children[i] != NULL) {
                if (class_add_child(self->children[i], parent, child)) {
                    return true;
                }
            }
        }
    }
  return false;
}
// fonction rajouté

bool hierarchy_add_child(struct oo_hierarchy *self, const char *parent, const char *child) {
    if (hierarchy_has_class(self, child)) {
        return false;
    }
  return class_add_child(self->root,parent,child);
}
bool class_is_child_of(const struct oo_class *self, const char *parent){
  if(self->parent == NULL){
    return false;
  }
  if(strcmp(self->parent->name,parent)==0){
    return true;
  }

  return class_is_child_of(self->parent,parent);
}
bool class_is_child_of_allTree(const struct oo_class *self, const char *parent,const char *child){
    if (self != NULL) {
        if (strcmp(self->name, child) == 0) {
            return class_is_child_of(self, parent);
        }


        for (size_t i = 0; i < self->size; ++i) {
            if (self->children[i] != NULL) {
                if (class_is_child_of_allTree(self->children[i], parent, child)) {
                    return true;
                }
            }
        }
    }
  return false;
}
bool hierarchy_is_child_of(const struct oo_hierarchy *self, const char *parent, const char *child) {

  return class_is_child_of_allTree(self->root,parent,child);
}
bool class_delete(struct oo_class *self){
  if (self != NULL) {
      for (size_t i = 0; i < self->size; ++i) {
          if (self->children[i] != NULL) {
              class_delete(self->children[i]);
          }
      }
      for (size_t i = 0; i < self->parent->size; ++i) {
          if (strcmp(self->parent->children[i]->name, self->name) == 0) {
              self->parent->children[i] = NULL;
          }
      }
      free(self);
      return true;
  }
  
  return false;
}
bool class_delete_in_tree(struct oo_class * self, const char* name) {
    if (self != NULL) {
        if (strcmp(self->name, name) == 0) {
            return class_delete(self);
        }


        for (size_t i = 0; i < self->size; ++i) {
            if (self->children[i] != NULL) {
                if (class_delete_in_tree(self->children[i], name)) {
                    return true;
                }
            }
        }
    }
    return false;
}
bool hierarchy_delete(struct oo_hierarchy *self, const char *name) {
  return class_delete_in_tree(self->root,name);
}
bool class_rename(struct oo_class *self, const char *old_name, const char *new_name){
    
    if (self != NULL) {
        if (strcmp(self->name, old_name) == 0) {
            libererChaineNom(self->name);
            self->name = creerChaineNom(new_name);
            return true;
        }
        for (size_t i = 0; i < self->size; ++i) {
            if (self->children[i] != NULL) {
                if (class_rename(self->children[i], old_name, new_name)) {
                    return true;
                }
            }
        }
    }
  return false;
}
bool hierarchy_rename(struct oo_hierarchy *self, const char *old_name, const char *new_name){
    if (hierarchy_has_class(self, new_name)) {
        return false;
    }
  return class_rename(self->root,old_name,new_name);
}

bool hierarchy_move_as_child_of(struct oo_hierarchy *self, const char *name, const char *parent){
  return false;//hierarchy_move();
}
bool path_possible(struct oo_hierarchy* self, const char* path) {
    const char delim[2] = "/";
    char* token;
    token = strtok(path, delim);

    if (strcmp(token, "Object") == 0) {
        token = strtok(NULL, delim);
    }

    while (token != NULL) {
        token = strtok(NULL, delim);
    }
    return true;
}
bool class_add_path(struct oo_class* self, const char* path) {
    if (self == NULL) {
        return false;
    }
    const char delim[2] = "/";
    char* token;
    token = strtok(path, delim);

    if (strcmp(token, "Object") == 0) {
        token = strtok(NULL, delim);
    }

    struct oo_class curr = *self;

    while (token != NULL) {
        class_add(self, token);
        curr = *self->children[0];
        token = strtok(NULL, delim);
    }

    return true;
}

bool check_hierarchy_add_path(struct oo_hierarchy* self, const char* path) {
    const char delim[2] = "/";
    char* token;
    token = strtok(path, delim);
    struct oo_class* curr = self->root;
    while (token != NULL && curr != NULL && hierarchy_has_class(self, token)) {
        curr = getChild(curr, token);
        token = strtok(NULL, delim);
    }
    bool duplicated = false;
    while (token != NULL && curr != NULL && hierarchy_has_class(self,  token)!=true) {
        token = strtok(NULL, delim);
    }

    if (token == NULL) {
        return true;
    }
    else {
        return false;
    }
}

bool hierarchy_add_path(struct oo_hierarchy *self, const char *path) {
    if (check_hierarchy_add_path(self, path) == false) {
        return false;
    }
    const char delim[2] = "/";
    char* token;
    token = strtok(path, delim);
    struct oo_class *curr = self->root;
    while (token != NULL && curr !=NULL) {
        hierarchy_add_child(self,curr->name,token);
        curr = getChild(curr, token);
        token = strtok(NULL, delim);
    }
    if (curr == NULL) {
        return false;
    }
    else {
        return true;
    }
}

bool class_add_path_as_child_of(struct oo_class *self, const char *path, const char *parent){
    if (self != NULL) {
        if (strcmp(self->name, parent) == 0) {
            const char delim[2] = "/";
            char* token;
            token = strtok(path, delim);
            while (token != NULL) {
                if (has_child_named(self, token)) {
                    return false;
                }
                token = strtok(NULL, delim);
            }

            return class_add_path(self, path);
        }
        for (size_t i = 0; i < self->size; ++i) {
            if (self->children[i] != NULL) {
                if (class_add_path_as_child_of(self->children[i], path, parent)) {
                    return true;
                }
            }
        }
    }
  return false;
}
bool hierarchy_add_path_as_child_of(struct oo_hierarchy *self, const char *path, const char *parent){
  return class_add_path_as_child_of(self->root, path, parent);
}

char *class_get_path_to(const struct oo_class *self, const char *name){
  char* res = NULL;

  if(strcmp(self->name,name)==0){
    struct oo_class *curr = self;
    do{
        if (strcmp(curr->name, "Object") == 0) {
            res = strcatEN_MIEUX(res, curr->name, "");
        }
        else {
            res = strcatEN_MIEUX(res, curr->name, "/");
        }
        
        curr = curr->parent;

    }while(curr!=NULL);
    
    return res;
  }

  for(size_t i=0; i<self->size && res!=NULL ; ++i){
      if (self->children[i] != NULL) {
          res = class_get_path_to(self->children[i], name);
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
  printf("%s\n",class_get_path_to(self,self->name));
  for(size_t i=0; i<self->size ; ++i){
      if (self->children[i] != NULL) {
          class_print(self->children[i], out);
      }
  }
}
void hierarchy_print(const struct oo_hierarchy *self, FILE *out) {
  class_print(self->root,out);
}

bool hierarchy_move(struct oo_hierarchy *self, const char *origin, const char *destination) {
  return false;
}
int main() {
    struct oo_hierarchy hierarchy;

    hierarchy_create(&hierarchy);


    hierarchy_add_child(&hierarchy, "Object", "Enfant0");
    hierarchy_add_child(&hierarchy, "Object", "Enfant1");
    hierarchy_add_child(&hierarchy, "Enfant0", "Enfant01");
    hierarchy_add_child(&hierarchy, "Enfant1", "Enfant10");
    hierarchy_add_child(&hierarchy, "Enfant1", "Enfant11");
    hierarchy_add_child(&hierarchy, "Enfant11", "Enfant110");
    hierarchy_add_child(&hierarchy, "Enfant11", "Enfant111");
    hierarchy_add_child(&hierarchy, "Enfant111", "Enfant1110");
    hierarchy_print(&hierarchy, stdout);
    





    printf("%d\n", hierarchy_count_classes(&hierarchy));
    printf("%s\n", hierarchy_has_class(&hierarchy, "Object") ? "true" : "false");
    // ecrire en ternaire pour toutes les fct renvoyant un booleen
    printf("%s\n", hierarchy_add_child(&hierarchy, "Object", "String") ? "true" : "false");
    // besoin de printf pour les hierarchy add child ?
    printf("%s\n", hierarchy_add_child(&hierarchy, "Object", "Number") ? "true" : "false");
    printf("%s\n", hierarchy_add_child(&hierarchy, "Object", "Throwable") ? "true" : "false");
    printf("%s\n", hierarchy_add_child(&hierarchy, "Number", "Double") ? "true" : "false");
    printf("%s\n", hierarchy_add_child(&hierarchy, "Number", "Integer") ? "true" : "false");
    printf("%s\n", hierarchy_add_child(&hierarchy, "Throwable", "Exception") ? "true" : "false");
    printf("%s\n", hierarchy_add_child(&hierarchy, "Throwable", "Error") ? "true" : "false");
    
    hierarchy_print(&hierarchy, stdout);

    printf("%d\n", hierarchy_count_classes(&hierarchy)); // doit renvoyer 8
    printf("%s\n", hierarchy_has_class(&hierarchy, "String") ? "true" : "false"); // doit renvoyer true;

    printf("%s\n", hierarchy_is_child_of(&hierarchy, "Object", "Number") ? "true" : "false"); // doit renvoyer true
    printf("%s\n", hierarchy_is_child_of(&hierarchy, "Number", "Double") ? "true" : "false"); // doit renvoyer true
    printf("%s\n", hierarchy_is_child_of(&hierarchy, "Double", "Integer") ? "true" : "false"); // doit renvoyer false
    printf("%s\n", hierarchy_rename(&hierarchy, "Integer", "Integer2") ? "true" : "false");
    hierarchy_print(&hierarchy, stdout);
    
    printf("%s\n", hierarchy_has_class(&hierarchy, "Integer2") ? "true" : "false"); // doit renvoyer true si le rename a bien fonctionné
    printf("%s\n", hierarchy_delete(&hierarchy, "Integer2") ? "true" : "false");
    hierarchy_print(&hierarchy, stdout);
    printf("%d\n", hierarchy_count_classes(&hierarchy)); // doit renvoyer 7
    printf("%s\n", hierarchy_has_class(&hierarchy, "Integer2") ? "true" : "false"); // doit renvoyer false;
    hierarchy_print(&hierarchy, stdout);
    hierarchy_destroy(&hierarchy);

    return 0;
}

