#include "native.h"

//
// List
//

lval* native_head(lenv* e, lval* a) {
  LASSERT_ARGS(a, "head", 1);
  LASSERT_TYPE(a, "head", 0, LVAL_QEXPR);
  LASSERT(a, a->cell[0]->count != 0,
    "Function 'head' passed empty Q-expression!");

  // Take the first argument
  lval* v = lval_take(a, 0);

  // Delete all arguments that aren't the head
  while (v->count > 1) { lval_del(lval_pop(v, 1)); }
  return v;
}

lval* native_tail(lenv* e, lval* a) { 
  LASSERT_ARGS(a, "tail", 1);
  LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
    "Function 'head' passed incorrect type!");
  LASSERT(a, a->cell[0]->count != 0,
    "Function 'head' passed empty Q-expression!");
  
  // Take the first argument
  lval* v = lval_take(a, 0);
  
  // Delete the first element and return;
  lval_del(lval_pop(v, 0));
  return v;
}

lval* native_list(lenv* e, lval* a) {
  a->type = LVAL_QEXPR;
  return a;
}

lval* native_eval(lenv* e, lval* a) {
  LASSERT_ARGS(a, "eval", 1);
  LASSERT_TYPE(a, "eval", 0, LVAL_QEXPR);
    
  lval* x = lval_take(a, 0);
  x->type = LVAL_SEXPR;
  return lval_eval(e, x);
}

lval* native_join(lenv* e, lval* a) {
  if (a->cell[0]->type == LVAL_STR) {
    // Handle string joining
    int length = 0;
    for (int i = 0; i < a->count; i++) {
      LASSERT_TYPE(a, "join", i, LVAL_STR);
      length += strlen(a->cell[i]->str);
    }

    char* str = calloc(1, length + 1);
    for (int i = 0; i < a->count; i++) {
      strcat(str, a->cell[i]->str);
    }

    lval* x = lval_str(str);
    free(str); // lval_str copies the contents and holds a new char*

    lval_del(a);
    return x;
  }

  // Joining a Q-expression
  for (int i = 0; i < a->count; i++) {
    LASSERT_TYPE(a, "join", i, LVAL_QEXPR);
  }
  
  lval* x = lval_pop(a, 0);
  
  while (a->count) {
    x = lval_join(x, lval_pop(a, 0));
  }
  
  lval_del(a);
  return x;
}

lval* native_cons(lenv* e, lval* a) {
  LASSERT_ARGS(a, "cons", 2);
  LASSERT_TYPE(a, "cons", 1, LVAL_QEXPR);
  
  // Make a new Q-expr, add the first value to it
  lval* v = lval_qexpr();
  lval_add(v, lval_pop(a, 0));

  // Join it with the old list
  v = lval_join(v, lval_pop(a, 0));

  // Delete old parent
  lval_del(a);

  return v;
}

lval* native_len(lenv* e, lval* a) {
  LASSERT_ARGS(a, "len", 1);
  LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
    "Function 'len' passed incorrect type!");
    
  int c = a->cell[0]->count;

  lval_del(a);
  return lval_long(c);
}
