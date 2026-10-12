#include "string_util.h"
#include "my_math.h"

int string_size(char *a){
    int i = 0;
    while(a[i] != '\0'){
        i ++;
    }
    return i;
}

void copy_string(char *a, char *b){
    int i = 0;
    while(a[i] != '\0'){
        b[i] = a[i];
        i ++;
    }
    b[i] = '\0';
}

void string_append(char *a, char *b){
    int i = 0;
    while(a[i] != '\0') i++;
    int j = 0;
    while(b[j] != '\0'){
        a[i] = b[j];
        i++;
        j++;
    }
    a[i] = '\0';
}

int string_compare(char *a, char *b){
    int i = 0;
    while(1){
        if(a[i] == '\0'){
            if(b[i] == '\0') return 1;
            else return 0;
        }
        if(a[i] != b[i]) return 0;
        else i++;
    }
}

int char_to_int(char *a){
    int i = 0;
    while(a[i] != '\0'){
        i++;
    }
    int result = 0;
    for(int j = 0; j < i; j++){
        int n = a[j] - '0';
        if(0 <= n && n <= 9){
            result += n * power(10, i - (j + 1));
        }
        else return -1;
    }
    return result;
}

double char_to_double(char *a){
    int ent = 0;
    int frac = 0;
    int flag = 0;
    int i = 0;
    while(a[i] != '\0'){
        if(a[i] == '.'){
            flag =1;
            i++;
            continue;
        }
            if(flag)frac++;
            else ent++;
            i++;
        }
    double result = 0;
    for(int j = 0; j < ent; j++){
        double n = a[j] - '0';
        if(0 <= n && n <= 9){
            result += n * power(10,ent - j - 1);
        }
        else return -1;
    }
    for(int j = 0; j < frac; j++){
        double n = a[ent + j + 1] - '0';
        result += n * inv_power(10, j + 1);
    }
    return result;
}