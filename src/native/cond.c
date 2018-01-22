#include "native.h"
#include "lcontext.h"

//
// Conditional
//
lval* native_eq(lenv* e, lval* a) {
  LASSERT_ARGS(a, "==", 2);
  
  int r = lval_eq(a->cell[0], a->cell[1]);
  lval_del(a);
  return lval_long(r);
}

lval* native_ne(lenv* e, lval* a) {
  LASSERT_ARGS(a, "!=", 2);
  
  int r = !lval_eq(a->cell[0], a->cell[1]);
  lval_del(a);
  return lval_long(r);
}

lval* native_or(lenv* e, lval* a) {
  LASSERT_COND(a, "||");
  
  int r = a->cell[0]->num_l || a->cell[1]->num_l;
  lval_del(a);
  return lval_long(r);
}

lval* native_and(lenv* e, lval* a) {
  LASSERT_COND(a, "&&");
  
  int r = a->cell[0]->num_l && a->cell[1]->num_l;
  lval_del(a);
  return lval_long(r);
}

lval* native_not(lenv* e, lval* a) {
  LASSERT_ARGS(a, "!", 1);
  LASSERT_TYPE(a, "!", 0, LVAL_LONG);

  int r = !a->cell[0]->num_l;
  lval_del(a);
  return lval_long(r);
}

lval* native_if(lenv* e, lval* a) {
  LASSERT_ARGS(a, "if", 3);
  LASSERT_TYPE(a, "if", 0, LVAL_LONG);
  LASSERT_TYPE(a, "if", 1, LVAL_QEXPR);
  LASSERT_TYPE(a, "if", 2, LVAL_QEXPR);
  
  lval* r;
  a->cell[1]->type = LVAL_SEXPR;
  a->cell[2]->type = LVAL_SEXPR;

  // Get eval environment
  lcontext* ctx = lenv_get_eval(e);
  
  if (a->cell[0]->num_l) {
    r = lcontext_eval(ctx, lval_pop(a, 1));
  } else {
    r = lcontext_eval(ctx, lval_pop(a, 2));
  }
  
  lval_del(a);
  return r;
}