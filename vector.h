#ifndef VECTOR
#define VECTOR

#include <stdlib.h>

struct Vector{
    int len;
    double *values;
};

void init_Vector(struct Vector *v, int len);

struct Vector NULL_Vector();

void free_Vector(struct Vector *v);

void print_Vector(struct Vector *v);

#endif
