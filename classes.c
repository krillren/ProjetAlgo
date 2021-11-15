#include "classes.h"
#include <stdlib.h>
#include <string.h>



char * creerChaineNom(const char* nom) {
    if(nom!=NULL){
        char * tmpc = malloc(strlen(nom) + 1);
        strcpy(tmpc, nom);

        return tmpc;
    }
    return NULL;
}

void libererChaineNom(char*nom) {
    if(nom!=NULL){
        free(nom);
    }
}
char * strcatEN_MIEUX(char *res, char *nom,const char * delim) {
    char* tmp=malloc(1);
    size_t tailleRes=0;
    size_t tailleNom = 0;
    size_t tailleDelim = 0;

    if (res != NULL && strlen(res) >=1 ) {
        tailleRes = strlen(res);
        free(tmp);
        tmp = malloc(tailleRes + 1);
        if (tmp != NULL) {
            strcpy(tmp, res);
            libererChaineNom(res);
        }
    }
    else {
        tailleRes = 0;
    }

    if (nom != NULL) {
        tailleNom = strlen(nom);
        if (delim != NULL && strlen(delim) >= 1) {
            tailleDelim = 1;
            
        }
        res = malloc(tailleDelim +tailleNom + tailleRes+1);

        if (res!=NULL ) {
            if(tailleDelim>=1){
                 strncpy(res, delim, tailleDelim);
            }
           
            strncpy(res + tailleDelim, nom, tailleNom);

            if ( tmp!=NULL) {
                strncpy(res + tailleDelim + tailleNom, tmp, tailleRes);
            }

            res[tailleDelim + tailleNom + tailleRes] = '\0';
        }

        libererChaineNom(tmp);
        return res;
    }
    

   
    libererChaineNom(tmp);
    return res;
}
bool DoublonChemin(const char* haystack,const char*needle){
    char* slashtoken =strcatEN_MIEUX("",needle,"/");
    bool res=false;
    if(strstr(strstr(haystack,needle)+strlen(needle),slashtoken)!=NULL){
        res=true;
    }
    libererChaineNom(slashtoken);
    return res;
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

bool class_delete(struct oo_class *self){
  if (self != NULL) {
    for (size_t i = 0; i < self->size; ++i) {
        class_delete(self->children[i]);
    }
    if(self->parent!=NULL){
        for (size_t i = 0; i < self->parent->size; ++i) {

            if(self->parent->children[i]!=NULL){
                if (strcmp(self->parent->children[i]->name, self->name) == 0) {
                    self->parent->children[i] = NULL;
                }
            }
                
        }
    }
      
      libererChaineNom(self->name);
      free(self->children);
      free(self);
      return true;
  }
  
  return false;
}
void hierarchy_destroy(struct oo_hierarchy *self) {
  class_delete(self->root);
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
    if(strcmp(self->name, name) == 0){
        return true;
    }
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
void class_add_exist(struct oo_class* dest, struct oo_class* src) {
    if (dest->size == dest->capacity) {
        dest->capacity *= 2;
        struct oo_class** children = calloc(dest->capacity, sizeof(struct oo_class*));
        if (children != NULL) {
            memcpy(children, dest->children, dest->size * sizeof(struct oo_class*));
            free(dest->children);
            dest->children = children;
        }
    }
    if (dest->children != NULL) {
        dest->children[dest->size] = src;
        src->parent = dest;
        dest->size += 1;
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

bool class_delete_in_tree(struct oo_class * self, const char* name) {
    if (self != NULL && strcmp(name,"Object")!=0) {
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
    if(strcmp(old_name,new_name)==0){
        return true;
    }
    if (hierarchy_has_class(self, new_name)||strcmp(old_name,"Object")==0 ) {
        return false;
    }
  return class_rename(self->root,old_name,new_name);
}
struct oo_class* find_in_class(struct oo_class* self, const char* name){
   
    if (self != NULL && name != NULL) {
        if (strcmp(self->name, name) == 0) {
            return self;
        }

        for (size_t i = 0; i < self->size; ++i) {
            if (self->children[i] != NULL) {
                struct oo_class* res = find_in_class(self->children[i], name);
                if (res!=NULL) {
                    return res;
                }
            }
        }
    }
    return NULL;
}
struct oo_class* find_in_hierarchy(struct oo_hierarchy* self, const char* name) {
    
    return find_in_class(self->root, name);
}
bool hierarchy_move_as_child_of(struct oo_hierarchy *self, const char *name, const char *parent){
    struct oo_class* src = find_in_hierarchy(self,name);
    struct oo_class* dest = find_in_hierarchy(self, parent);
    if (src == NULL || dest == NULL || src->parent==NULL||src == dest) {
        return false;
    }
    for (size_t i = 0; i < src->parent->size; ++i) {
        if (strcmp(src->parent->children[i]->name, src->name) == 0) {
            src->parent->children[i] = NULL;
        }
    }
    class_add_exist(dest, src);
    return true;
}


bool check_hierarchy_add_path(struct oo_hierarchy* self, const char* path) {
    const char delim[2] = "/";
    
    char* copy = creerChaineNom(path);
    char* token=NULL;
    token = strtok(copy, delim);
    if(token==NULL){
        libererChaineNom(copy);
        return false;
    }
    
    if (strcmp(token, "Object") == 0) {
        token = strtok(NULL, delim);
    }
    struct oo_class* curr = self->root;

    while (token != NULL && curr != NULL && hierarchy_has_class(self, token)) {
        if(DoublonChemin(path,token)){
            libererChaineNom(copy);
            return false;
        }
        curr = getChild(curr, token);
        token = strtok(NULL, delim);
    }
    while (token != NULL && !(hierarchy_has_class(self,  token))) {
        if(DoublonChemin(path,token)){
            libererChaineNom(copy);
            return false;
        }
        token = strtok(NULL, delim);
    }
    
    if (token == NULL) {
        libererChaineNom(copy);
        return true;
    }
    libererChaineNom(copy);
    return false;
}

bool hierarchy_add_path(struct oo_hierarchy *self, const char *path) {
    if (!(check_hierarchy_add_path(self, path))) {
        return false;
    }
    bool pour_les_tests=false;
    const char delim[2] = "/";
    char* token;
    char* copy = creerChaineNom(path);
    token = strtok(copy, delim);
    struct oo_class *curr = self->root;
    if (strcmp(token, "Object") == 0) {
        token = strtok(NULL, delim);
    }
    while (token != NULL && curr !=NULL) {
        if(hierarchy_add_child(self,curr->name,token)){
            pour_les_tests=true;
        }
        curr = getChild(curr, token);
        token = strtok(NULL, delim);
    }
    libererChaineNom(copy);
    if (curr == NULL) {
        return false;
    }
    return pour_les_tests;
}


bool check_class_add_path_as_child(struct oo_hierarchy*hierarchy, const char* path) {
    const char delim[2] = "/";
    char* copy = creerChaineNom(path);
    char* token = NULL;
    token = strtok(copy, delim);
    if (strcmp(token, "Object") == 0) {
        token = strtok(NULL, delim);
    }
    while (token != NULL && hierarchy_has_class(hierarchy, token) != true) {
        if(DoublonChemin(path,token)){
            libererChaineNom(copy);
            return false;
        }
        token = strtok(NULL, delim);
    }
    if (token == NULL) {
        libererChaineNom(copy);
        return true;
    }
    libererChaineNom(copy);
    return false;
}
bool class_add_path_as_child_of(struct oo_hierarchy* hierarchy ,struct oo_class *self, const char *path, const char *parent){
    
    if (self != NULL) {
        if (strcmp(self->name, parent) == 0) {
            if (!(check_class_add_path_as_child(hierarchy,path))) {
                return false;
            }
            const char delim[2] = "/";
            char* token;
            char* copy = creerChaineNom(path);
            token = strtok(copy, delim);
            struct oo_class* curr = self;
            while (token != NULL && curr != NULL) {
                hierarchy_add_child(hierarchy, curr->name, token);
                curr = getChild(curr, token);
                token = strtok(NULL, delim);
            }
            libererChaineNom(copy);
            if (curr == NULL) {
                return false;
            }
            return true;

        }
        for (size_t i = 0; i < self->size; ++i) {
            if (self->children[i] != NULL) {
                if (class_add_path_as_child_of(hierarchy,self->children[i], path, parent)) {
                    return true;
                }
            }
        }
    }
    
  return false;
}
bool hierarchy_add_path_as_child_of(struct oo_hierarchy *self, const char *path, const char *parent){
  return class_add_path_as_child_of(self,self->root, path, parent);
}

char *class_get_path_to(const struct oo_class *self, const char *name){
  char* res = NULL;

  if(strcmp(self->name,name)==0){
    const struct oo_class *curr = self;
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

  for(size_t i=0; i<self->size && res==NULL ; ++i){
      if (self->children[i] != NULL) {
          res = class_get_path_to(self->children[i], name);
      }
  }

  return res;
}
char *hierarchy_get_path_to(const struct oo_hierarchy *self, const char *name) {
  return class_get_path_to(self->root,name);
}



/*
* fonction permet de comparer deux types classes léxicographiquement, en gérant les cas ou les classes sont NULL.
* on considére qu'une classe NULL est classé après les classes non-vides.
*/
bool class_compare_lexico(struct oo_class* self1, struct oo_class* self2) {
    if (self1 == self2 ) {
        return false;
    }
    else if (self1 == NULL) {
        return true;
    }
    else if (self2 == NULL) {
        return false;
    }
    else if (strcmp(self1->name, self2->name) < 0) {
        return true;
    }
    return false;
}
void MYarray_swap(struct oo_class** children, size_t i, size_t j) {
    
    struct oo_class* tmp = children[i];
    children[i] = children[j];
    children[j] = tmp;
    
}
ptrdiff_t MYarray_partition(struct oo_class** children, ptrdiff_t i, ptrdiff_t j) {
    ptrdiff_t pivot_index = i;
    struct oo_class* pivot = children[pivot_index];
    MYarray_swap(children, pivot_index, j);
    ptrdiff_t l = i;
    for (ptrdiff_t k = i; k < j; ++k) {
        if (class_compare_lexico(children[k],pivot)) {
            MYarray_swap(children, k, l);
            ++l;
        }
    }
    MYarray_swap(children, l, j);
    return l;
}
void MYarray_quick_sort_partial(struct oo_class** children, ptrdiff_t i, ptrdiff_t j) {
    if (i < j) {
        ptrdiff_t p = MYarray_partition(children, i, j);
        MYarray_quick_sort_partial(children, i, p - 1);
        MYarray_quick_sort_partial(children, p + 1, j);
    }
}
void MYarray_quick_sort(struct oo_class** children, size_t n) {
    if (children==NULL || n == 0) {
        return;
    }
    MYarray_quick_sort_partial(children, 0, n - 1);
}

void class_sort(struct oo_class* self) {
    MYarray_quick_sort(self->children, self->size);
    for (size_t i = 0; i < self->size; ++i) {
        if (self->children[i] != NULL) {
            class_sort(self->children[i]);
        }
        
    }
}
void hierarchy_sort(struct oo_hierarchy *self) {
    class_sort(self->root);
}




void class_print(const struct oo_class *self, FILE *out){
    char * res = class_get_path_to(self,self->name);
    if(res!=NULL){
        fprintf(out,"%s\n",res);
    }
  
  libererChaineNom(res);
  for(size_t i=0; i<self->size ; ++i){
      if (self->children[i] != NULL) {
          class_print(self->children[i], out);
      }
  }
}
void hierarchy_print(const struct oo_hierarchy *self, FILE *out) {
  class_print(self->root,out);
}



char* check_origin_path(struct oo_hierarchy *self, const char *origin){
    const char delim[2] = "/";
    
    char* copy = creerChaineNom(origin);
    char* token=NULL;
    char* token2=NULL;
    token = strtok(copy, delim);
    if(token==NULL){
        libererChaineNom(copy);
        return NULL;
    }
    
    if (strcmp(token, "Object") == 0) {
        token = strtok(NULL, delim);
    }
    struct oo_class* curr = self->root;

    while (token != NULL && curr != NULL) {
        if(DoublonChemin(origin,token)){
            libererChaineNom(copy);
            libererChaineNom(token2);
            return NULL;
        }
        libererChaineNom(token2);
        token2=creerChaineNom(token);
        curr = getChild(curr, token);
        token = strtok(NULL, delim);
    }
    
    if (token == NULL&&curr!=NULL) {
        libererChaineNom(copy);
        return token2;
    }
    libererChaineNom(token2);
    libererChaineNom(copy);
    return NULL;
}
char* check_destination_path(struct oo_hierarchy *self, const char* destination, char* new_name){
    const char delim[2] = "/";
    
    char* copy = creerChaineNom(destination);
    char* token=NULL;
    token = strtok(copy, delim);
    if(token==NULL){
        libererChaineNom(copy);
        return NULL;
    }
    
    if (strcmp(token, "Object") == 0) {
        token = strtok(NULL, delim);
    }
    struct oo_class* curr = self->root;
    while (token != NULL && curr != NULL && getChild(curr, token)!=NULL ) {
        if(DoublonChemin(destination,token)){
            libererChaineNom(copy);
            return NULL;
        }
        
        if (getChild(curr, token)!=NULL){
            curr = getChild(curr, token);
            token = strtok(NULL, delim);
        }
        
    }
    
    char * copytoken = creerChaineNom(token);
    
    if (token != NULL && strtok(NULL,delim)==NULL) {

        strcpy(new_name,copytoken);
        libererChaineNom(copy);
        return curr->name;
    }
    libererChaineNom(copy);
    return NULL;
}

bool hierarchy_move(struct oo_hierarchy *self, const char *origin, const char *destination) {
    if(strcmp(origin,destination)==0){
        return true;
    }
    char * src =check_origin_path(self,origin);
    char * new_name = malloc(sizeof(destination));
    const char * dest =check_destination_path(self,destination,new_name);

    if(dest==NULL || src == NULL || (hierarchy_has_class(self,new_name) && strcmp(src,new_name)!=0)){
        return false;
    }
    
    bool done = hierarchy_move_as_child_of(self,src,dest) && hierarchy_rename(self,src,new_name);
    

    libererChaineNom(src);
    return done;
}


