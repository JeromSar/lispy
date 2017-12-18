#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#define STACK_SIZE 512

typedef struct stack stack;
struct stack {
    int len;
    int* syms;
};

stack* stack_new();
bool stack_push(stack* st, int sym);
int stack_pop(stack* st);
void stack_del(stack* st);

#endif
