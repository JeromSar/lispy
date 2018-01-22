#ifndef LENV_H
#define LENV_H

#include "lval.h"
#include "lcontext.h"

// Management
lenv* lenv_new(void);
void lenv_del(lenv* e);
lenv* lenv_copy(lenv* e);

// Operations
lval* lenv_get(lenv* e, lval* k);
void lenv_def(lenv* e, lval* k, lval* v);
void lenv_put(lenv* e, lval* k, lval* v);
void lenv_put_native(lenv* e, char* name, lnative func);
lcontext* lenv_get_eval(lenv* e);

#endif