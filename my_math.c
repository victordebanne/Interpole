#include "my_math.h"

int power(int a, int b){
    int total = 1;
    for(int i = 0; i < b; i++){
        total *= a;
    }
    return total;
}

double inv_power(int a, int b){
    return 1.0 / power(a, b);
}