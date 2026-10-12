//chord1=[1, 2, 5], chord2=[3,6,8],
// steps=10, function=exp, k=1\0
//mode = midi to freq
//mode = notes to midi 
//mode = micronotes to freq
//mode = freq to micronotes

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "string_util.h"
#include "vector.h"
#include "my_math.h"
#include "algo.h"
#include "chained_list.h"

struct TypeArg{
    char *type;
    char *arg;
};

struct Parsed{
    int len;
    struct TypeArg *types_args;
};

struct Args{
    struct Vector chord1;
    struct Vector chord2;
    int steps;
    char *function;
    double k;
    char *modein;
    char *modeout;
};

void free_Args(struct Args *a){
    if(a->chord1.values != NULL) free_Vector(&a->chord1);
    if(a->chord2.values != NULL) free_Vector(&a->chord2);
}

void init_TypeArg(struct TypeArg *ta){
    ta->type = malloc(100 * sizeof(char));
    ta->arg = malloc(100 * sizeof(char));
}

void init_Parsed(struct Parsed *p){
    p->len = 0;
    p->types_args = malloc(100 * sizeof(struct TypeArg));
    for(int i = 0; i < 100; i++){
        init_TypeArg(&p->types_args[i]);
    }
}

void free_TypeArg(struct TypeArg *ta){
    if(ta->type != NULL) free(ta->type);
    if(ta->arg != NULL) free(ta->arg);
}

void free_Parsed(struct Parsed *p){
    for(int i = 0; i < 100; i++){
        free_TypeArg(&p->types_args[i]);
    }
    if (p->types_args != NULL) free(p->types_args);
}

struct Parsed parser(char *input){
    //ignore les virgules entre crochets 
    //enleve les espaces
    //sépare type de l'argilent (=)
    //les arguments sont tous au format char *

    struct Parsed p;
    init_Parsed(&p);
    int p_i = 0;
    int i = 0;
    int index = 0;
    char buffer[100];

    while(input[i] != '\0'){
        
        if(input[i] == ' '){
            i++;
            continue;
        }
        else if(input[i] == ','){
            buffer[index] = '\0';
            copy_string(buffer, p.types_args[p_i].arg);
            index = 0;
            i++;
            p_i++;
            p.len++;
            continue;
        }
        else if(input[i] == '='){
            buffer[index] = '\0';
            copy_string(buffer, p.types_args[p_i].type);
            index = 0;
            i++;
            continue;
        }
        else if(input[i] == '['){
            //charger arg jusqu'a ']'
            i++;
            while(input[i] != ']'){
                buffer[index] = input[i];
                i++;
                index++;
                if(index > 100 || input[i] == '\0'){
                    free_Parsed(&p);
                    p.len = -1;
                    return p;
                }
            }
            i++;
        }
        else{
            buffer[index] = input[i];
            index ++;
            i++;
        }
    }

    //dernier vidage du buffer
    buffer[index] = '\0';
    copy_string(buffer, p.types_args[p_i].arg);
    p.len++; 
    return p;
}

void print_Parsed(struct Parsed *p){
    for(int i = 0; i < p->len; i++){
        printf("%s = ", p->types_args[i].type);
        printf("%s\n", p->types_args[i].arg);
    }
}

struct Vector list_parser(char *a){
    struct Vector v;
    init_Vector(&v, 100);

    int i = 0;
    char buffer[100];
    int index = 0;
    int v_i = 0;
    v.len = 0;

    while(a[i] != '\0'){
        if(a[i] == ' '){
            i++;
            continue;
        }
        else if(a[i] == ','){
            buffer[index] = '\0';
            double x = char_to_double(buffer);
            if(x < 0){
                free_Vector(&v);
                return NULL_Vector(); 
            }
            v.values[v_i] = x;
            index = 0;
            v_i++;
            v.len++;
            i++;
        }
        else{
            buffer[index] = a[i];
            index++;
            i++;
        }
    }
    buffer[index] = '\0';
    double x = char_to_double(buffer);
    v.values[v_i] = x;
    v.len++;

    return v;
}

struct Vector notelist_parser(char *a){
    struct CharChainedList c;
    init_CharChainedList(&c);

    char *notes_sharp[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    char *notes_flat[] = {"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"};

    int i = 0;
    char buffer[100];
    int index = 0;
    int size = 0;

    while(a[i] != '\0'){
        if(a[i] == ' '){
            i++;
            continue;
        }
        else if(a[i] == ','){
            buffer[index] = '\0';
            append(&c, buffer);
            size++;
            index = 0;
            i++;
        }
        else{
            buffer[index] = a[i];
            index++;
            i++;
        }
    }
    buffer[index] = '\0';
    append(&c, buffer);
    size++;

    struct Vector v;
    init_Vector(&v, size);

    for(int j = 0; j < size; j++){
        char *note = get(&c, j);
        int n = string_size(note);
        int octave;
        char gamme[3];
        int pitch;
        if(n == 2){
            octave = note[1] - '0';
            gamme[0] = note[0];
            gamme[1] = '\0';
        }
        else if(n == 3){
            octave = note[2] - '0';
            gamme[0] = note[0];
            gamme[1] = note[1];
            gamme[2] = '\0';
        }
        else{
            free_Vector(&v);
            free_CharChainList(&c);
            return NULL_Vector(); 
        } 
        int flag = 0;
        for(int k = 0; k < 12; k++){
            if(string_compare(gamme, notes_sharp[k])){
                pitch = k;
                flag = 1;
            }
        }
        for(int k = 0; k < 12; k++){
            if(string_compare(gamme, notes_flat[k])){
            pitch = k;
            flag = 1;
            }
        }
        if(flag == 0){
            free_Vector(&v);
            free_CharChainList(&c);
            return NULL_Vector(); 
        } 
        v.values[j] = octave * 12 + pitch;
    }
    free_CharChainList(&c);
    return v;
}

char **midi_to_note(struct  Vector *v){
    char *notes_sharp[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    char *notes_flat[] = {"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"};
    char *numbers[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    char **out = malloc(v->len * sizeof(char*));
    for(int i = 0; i < v->len; i++){
        int n = round(v->values[i]);
        int octave = n / 12;
        int pitch = n % 12;
        out[i] = malloc(4 * sizeof(char));
        copy_string(notes_sharp[pitch], out[i]);
        string_append(out[i], numbers[octave]);
    }
    return out;
}

struct Args fill_args(struct Parsed *p){
    struct Args a;
    a.chord1 = NULL_Vector();
    a.chord2 = NULL_Vector();
    a.steps = 1;
    a.function = "linear";
    a.k = 0;
    a.modein = "midi";
    a.modeout = "midi";

    for(int i = 0; i < p->len; i++){
        if(string_compare(p->types_args[i].type, "modein")) a.modein = p->types_args[i].arg;
    }

    for(int i = 0; i < p->len; i++){
        if(string_compare(p->types_args[i].type, "chord1")){
            if(string_compare(a.modein, "midi")) a.chord1 = list_parser(p->types_args[i].arg);
            else if(string_compare(a.modein, "note")) a.chord1 = notelist_parser(p->types_args[i].arg);
            else {
                printf("chord1\n");
                a.steps = -1;
                free_Args(&a);
                return a;
            }
        }
        else if(string_compare(p->types_args[i].type, "chord2")){
            if(string_compare(a.modein, "midi")) a.chord2 = list_parser(p->types_args[i].arg);
            else if(string_compare(a.modein, "note")) a.chord2 = notelist_parser(p->types_args[i].arg);
            else {
                printf("chord2\n");
                a.steps = -1;
                return a;
            }
            if(a.chord2.len < 0){
                printf("chord2\n");
                a.steps = -1;
                free_Args(&a);
                return a;
            }
        }
        else if(string_compare(p->types_args[i].type, "steps")){
            a.steps = char_to_int(p->types_args[i].arg);
            if(a.steps < 0){
                printf("steps\n");
                free_Args(&a);
                return a;
            } 
        }
        else if(string_compare(p->types_args[i].type, "function")){
            a.function = p->types_args[i].arg;
        }
        else if(string_compare(p->types_args[i].type, "k")){
            a.k = char_to_double(p->types_args[i].arg);
            if(a.k < 0){
                printf("k\n");
                a.steps = -1;
                free_Args(&a);
                return a;
            }
        }
        else if(string_compare(p->types_args[i].type, "modeout")){
            a.modeout = p->types_args[i].arg;
        }
        else if(string_compare(p->types_args[i].type, "modein")){
        }
        else {
            //erreur de type inconnu
            printf("%s\n", p->types_args[i].type);
            printf("type inconny\n");
            a.steps = -1;
            free_Args(&a);
            return a;
        }
    }
    if(a.chord1.len < 0){
                printf("chord1\n");
                a.steps = -1;
                free_Args(&a);
                return a;
            }
    if(a.chord2.len < 0){
                printf("chord2\n");
                a.steps = -1;
                free_Args(&a);
                return a;
            }
    return a;
}

void manual(){
    char *a = "\n"
        "bienvenu dans INTERPOLE, l'interpolateur d'accords.\n"
        "__________________________________________________\n"
        "cet outil en lignes de commandes permet de trouver des chemins \n"
        "entre deux accords, avec des règles linéaire ou exponentielles\n"
        "les détails mathématiques sont detailles sur ce lien.\n"
        "\n"
        "tous les parametres sont modifiables avec la relation : parametre = argument\n"
        "on peut separer chaque couple parametre/argument par une virgule\n"
        "les parametres modifiables sont : \n"
        "- les accords d'arrivés et de départ : chord1 et chord2\n"
        "- le nombre d'accords intermediaires : steps (entre 0 et +inf)\n"
        "- la fonction d'interpolation : function ('linear' ou 'exp'\n"
        "- la raideur de l'exponentielle : k (entre 0 et 1 exclu)\n"
        "- les modes d'entrés et de sorte : modein et modeout (midi ou note)\n"
        "\n"
        "Exemple d'une instruction : \n"
        "> chord1 = [36, 47, 58], chord2 = [38, 42, 51], steps = 4, function = exp, k = 0.5, modein = midi, modeout = midi\n"
        "les seules paramêtres obligatoires sont chord1 et chord2\n"
        "les autres paramêtres sont par défaut : steps = 1, function = linear, k = 0, modein = midi, modeout = midi\n"
        "on pourra noter que l'ordre des parametres n'a pas d'importance\n"
        "le mode 'modein=note' permet d'entrer des notes avec leurs octaves\n" 
        "le mode 'modeout=note' permet de traduire la sortie en notes\n" 
        "> chord1 = [C#1, D1, F#1], chord2 = [E2, G#2, A2], modein = midi, modeout = midi\n"
        "\n"
        "q sert à quitter le programme. "
        ;
    printf("%s\n", a);
}

int programme(){
    char input[1000];

    printf("> ");
    if(scanf(" %999[^\n]", input) != 1) return -4;
    if(string_compare(input, "q")) return -5;

    struct Parsed p = parser(input);
    if(p.len < 0) return -1;

    struct Args a = fill_args(&p);

    if(a.steps < 0) return -2;
    struct Vector *m = chord_interp(&a.chord1, &a.chord2, a.steps, a.function, a.k);
    if(m == NULL) return -6;
    
    if(string_compare(a.modeout, "midi")){
        for(int i = 0; i < a.chord1.len; i++){
            if(m[i].len < 0) return -3;
            print_Vector(&m[i]);
        }
    }
    else if(string_compare(a.modeout, "note")){
        for(int i = 0; i < a.chord1.len; i++){
            if(m[i].len < 0) return -3;
            char **notes = midi_to_note(&m[i]);
            for(int j = 0; j < a.steps + 2; j++){
                printf("%s ", notes[j]);
            }
            printf("\n");
        }
    }

    //LIBERATION MEMOIRE

    free_Parsed(&p);
    for(int i = 0; i < a.chord1.len; i++){
        if(m[i].values != NULL) free_Vector(&m[i]);
    }
    free(m);
    free_Args(&a);

    return 0;
}

int main(){
    manual();

    int a = 0;
    while(1){
        int a = programme();
        if(a == -1) printf("erreur parsing\n");
        else if(a == -2) printf("erreur arguments\n");
        else if(a == -3) printf("erreur vecteurs\n");
        else if(a == -5) break;
        else if(a == -6) printf("erreur accords\n");
    }
    return 0;
}