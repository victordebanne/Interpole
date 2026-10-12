#include <stdlib.h>
#include <stdio.h>
#include "vector.h"


void init_Vector(struct Vector *v, int len){
    v->len = len;
    v->values = malloc(len * sizeof(double));
}

struct Vector NULL_Vector(){
    struct Vector v;
    v.len = -1;
    v.values = NULL;
    return v;
}

void free_Vector(struct Vector *v){
    if(v->values != NULL) free(v->values);
}

void print_Vector(struct Vector *v){
    printf("[");
    for(int i = 0; i < v->len; i++){
        printf("%lf ", v->values[i]);
    }
    printf("]\n");
}
