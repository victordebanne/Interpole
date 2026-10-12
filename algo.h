#ifndef ALGO
#define ALGO

#include "vector.h"

struct Vector linear_interp(double a, double b, int steps);

struct Vector exp_interp(double a, double b, int steps, double k);

struct Vector interp(double a, double b, int steps, char *function, double k);

struct Vector *chord_interp(struct Vector *u, struct Vector *v, int steps, char *function, double k);

#endif