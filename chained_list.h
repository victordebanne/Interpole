#ifndef CHAINED_LIST
#define CHAINED_LIST

struct CharChainedList{
    char *value;
    struct CharChainedList *child;
};

void init_CharChainedList(struct CharChainedList *chain);

void append(struct CharChainedList *chain, char *value);

char *get(struct CharChainedList *chain, int i);

void free_CharChainList(struct CharChainedList *chain);

#endif