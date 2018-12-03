#ifndef STACK_H
#define STACK_H

typedef struct stack stack;
typedef struct stack_entry stack_entry;

#include <stdbool.h>

#include "symtable.h" // sym_loc
#include "lcontext.h" //lcontext
#include "lval.h" // lval

#define STACK_SIZE 512

struct stack {
  int len;
  stack_entry** entries;
};

struct stack_entry {
  char* sym;
  int row;
  int col;
};

stack* stack_new(void);
void stack_del(stack* st);
stack* stack_copy(stack* st);

bool stack_push_raw(stack* st, char* sym, int row, int col);
bool stack_push(stack* st, sym_loc* loc);
bool stack_push_lval(stack* st, lcontext* ctx, lval* val);
bool stack_pop(stack* st);
stack_entry* stack_peek(stack* st);

void stack_print(stack* st);

#endif
