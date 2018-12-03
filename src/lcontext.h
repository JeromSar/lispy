#ifndef EVAL_H
#define EVAL_H

typedef struct lcontext lcontext;

#include "lval.h"
#include "stack.h"
#include "lval.h"

struct lcontext {
  stack* stack;
  symtable* symtable;
  lenv* env;
};

lcontext* lcontext_new();
void lcontext_del(lcontext* l);
lval* lcontext_eval(lcontext* e, lval* v);
lval* lcontext_eval_sexpr(lcontext* e, lval* v);
lval* lcontext_call(lcontext* e, lval* f, lval* a);

// Auxiliary
void lcontext_print(lcontext* ctx);

#endif