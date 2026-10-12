#include <stdlib.h>
#include "chained_list.h"
#include "string_util.h"

void init_CharChainedList(struct CharChainedList *chain){
    chain->value = NULL;
    chain->child = NULL;
}

void append(struct CharChainedList *chain, char *value){
    if(chain->child != NULL){
        append(chain->child, value);
    }
    else{
        chain->value = malloc((string_size(value) + 1) * sizeof(char));
        copy_string(value, chain->value);
        
        chain->child = malloc(sizeof(struct CharChainedList));
        init_CharChainedList(chain->child);
    }
}

char *get(struct CharChainedList *chain, int i){
    if(i == 0){
        if(chain ->value == NULL) return NULL;
        else return chain->value;
    }
    else{
        if(chain->child != NULL) return get(chain->child, i - 1);
        else return NULL;
    }
}

void free_CharChainList(struct CharChainedList *chain){
    if(chain->value != NULL) free(chain->value);
    if(chain->child != NULL){
        free_CharChainList(chain->child);
        free(chain->child);
    }
}
