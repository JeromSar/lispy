#ifndef NATIVE_H
#define NATIVE_H

#include "../lval.h"
#include "../lenv.h"
#include "../eval.h"
#include "../util.h"
#include "../lispy.h"
#include "../grammar.h"
#include "../mpc/mpc.h"
#include "type.h"

#define LASSERT(args, cond, fmt, ...) \
  if (!(cond)) { \
    lval* err = lval_err(fmt, ##__VA_ARGS__); \
    lval_del(args); \
    return err; \
  }

#define LASSERT_ARGS(args, name, amount) \
  LASSERT( \
    (args), \
    (args)->count == (amount), \
    "Function '%s' passed incorrect amount of arguments. " \
    "Got %i, Expected %i.", \
    (name), \
    (args)->count, \
    (amount) \
  )

#define LASSERT_TYPE(args, name, index, atype) \
  LASSERT( \
    (args), \
    (args)->cell[(index)]->type == (atype), \
    "Function '%s' passed incorrect type for argument %i. " \
    "Got %s, Expected %s.", \
    (name), \
    (index), \
    lval_type_name((args)->cell[(index)]->type), \
    lval_type_name((atype)) \
  )
  
#define LASSERT_NUM(args, name, index) \
  LASSERT( \
    (args), \
    lval_is_num((args)->cell[(index)]), \
    "Function '%s' passed incorrect type for argument %i. " \
    "Got %s, Expected number type.", \
    (name), \
    (index), \
    lval_type_name((args)->cell[(index)]->type) \
  )
 
#define LASSERT_LONG(args, name, index) \
  LASSERT( \
    (args), \
    (args)->cell[(index)]->type == LVAL_LONG, \
    "Function '%s' passed incorrect type for argument %i. " \
    "Got %s, Expected long.", \
    (name), \
    (index), \
    lval_type_name((args)->cell[(index)]->type) \
  )

#define LASSERT_COND(args, name) \
  LASSERT_ARGS((args), (name), 2); \
  LASSERT_NUM((args), (name), 0); \
  LASSERT_NUM((args), (name), 1);

// native.c  
void native_addall(lenv* e);

// core.c
lval* native_def(lenv* e, lval* a);
lval* native_put(lenv* e, lval* a);
lval* native_isdef(lenv* e, lval* a);
lval* native_exit(lenv* e, lval* a);
lval* native_lambda(lenv* e, lval* a);
lval* native_load(lenv* e, lval* a);
lval* native_show(lenv* e, lval* a);
lval* native_print(lenv* e, lval* a);
lval* native_error(lenv* e, lval* a);
//lval* native_try(lenv* e, lval* a);

// math.c
lval* native_add(lenv* e, lval* a);
lval* native_sub(lenv* e, lval* a);
lval* native_mul(lenv* e, lval* a);
lval* native_div(lenv* e, lval* a);
lval* native_mod(lenv* e, lval* a);
lval* native_pow(lenv* e, lval* a);
lval* native_min(lenv* e, lval* a);
lval* native_max(lenv* e, lval* a);
lval* native_gt(lenv* e, lval* a);
lval* native_ge(lenv* e, lval* a);
lval* native_lt(lenv* e, lval* a);
lval* native_le(lenv* e, lval* a);

// cond.c
lval* native_eq(lenv* e, lval* a);
lval* native_ne(lenv* e, lval* a);
lval* native_or(lenv* e, lval* a);
lval* native_and(lenv* e, lval* a);
lval* native_not(lenv* e, lval* a);
lval* native_if(lenv* e, lval* a);

// list.c
lval* native_head(lenv* e, lval* a);
lval* native_tail(lenv* e, lval* a);
lval* native_list(lenv* e, lval* a);
lval* native_eval(lenv* e, lval* a);
lval* native_join(lenv* e, lval* a);
lval* native_cons(lenv* e, lval* a);
lval* native_len(lenv* e, lval* a);

// string.c
lval* native_str(lenv* e, lval* a);
// TODO:
//lval* native_atoi(lenv* e, lval* a);
//lval* native_formatf(lenv* e, lavl* a);

#endif
