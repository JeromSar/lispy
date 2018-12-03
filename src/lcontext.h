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

lval* lcontext_eval(lcontext* ctx, lenv* e, lval* v);
lval* lcontext_eval_sexpr(lcontext* ctx, lenv* e, lval* v);
lval* lcontext_call(lcontext* ctx, lenv* e, lval* f, lval* a);

#endif