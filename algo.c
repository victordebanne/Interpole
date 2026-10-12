#include <stdlib.h>
#include <math.h>

#include "string_util.h"
#include "vector.h"
#include "my_math.h"
#include "algo.h"


struct Vector linear_interp(double a, double b, int steps){
    if( a <= 0 || b <= 0) return NULL_Vector();
    double A = (b - a) / (steps + 1);
    double B = a;
    struct Vector v;
    init_Vector(&v, steps + 2);
    for(int i = 0; i < steps + 1; i++){
        v.values[i] = A * i + B;
    }
    v.values[steps + 1] = b;
    return v;
}   

struct Vector exp_interp(double a, double b, int steps, double k){
    if( a <= 0 || b <= 0 || k < 0 || k >= 1) return NULL_Vector();
    int I = 1;
    if(a > b) I = -1;
    double A = a * (I * (k - 1) + 1);
    double B = I * (a * (1 - k));
    double C = 1 + fabs(b - a)/ (a * (1 - k));
    struct Vector v;
    init_Vector(&v, steps + 2);
    for(int i = 0; i < steps + 1; i++){
        v.values[i] = A + B * pow(C,  (i * 1.0) / (steps + 1.0));
    }
    v.values[steps + 1] = b;
    return v;
}

struct Vector interp(double a, double b, int steps, char *function, double k){
    if(string_compare(function, "linear")){
        return linear_interp(a, b, steps);
    }
    else if(string_compare(function, "exp")){
        return exp_interp(a, b, steps, k);
    }
    else return NULL_Vector();
}

struct Vector *chord_interp(struct Vector *u, struct Vector *v, int steps, char *function, double k){
    if(u->len != v->len){
        return NULL;
    }
    struct Vector *m = malloc(u->len * sizeof(struct Vector));
    for(int i = 0; i < u->len; i++){
        m[i] = interp(u->values[i], v->values[i], steps, function, k);
    }
    return m;
}
