#include "lenv.h"
#include "lval.h"
#include "util.h" // streq

lenv* lenv_new(void) {
  lenv* e = malloc(sizeof(lenv));
  e->par = NULL;
  e->count = 0;
  e->syms = NULL;
  e->vals = NULL;
  return e;
}

void lenv_del(lenv* e) {
  for(int i = 0; i < e->count; i++) {
    free(e->syms[i]);
    lval_del(e->vals[i]);
  }
  free(e->syms);
  free(e->vals);
  free(e);
}

lenv* lenv_copy(lenv* e) {
  lenv* n = malloc(sizeof(lenv));
  n->par = e->par;
  n->count = e->count;
  n->syms = malloc(sizeof(char*) * n->count);
  n->vals = malloc(sizeof(lval*) * n->count);
  for (int i = 0; i < e->count; i++) {
    // Copy symbol
    n->syms[i] = malloc(strlen(e->syms[i]) + 1);
    strcpy(n->syms[i], e->syms[i]);
    
    // Copy value
    n->vals[i] = lval_copy(e->vals[i]);
  }
  return n;
}

lval* lenv_get(lenv* e, lval* k) {
  // Check local environment
  for (int i = 0; i < e->count; i++) {
    if (streq(e->syms[i], k->sym)) {
      // Return a copy
      return lval_copy(e->vals[i]);
    }
  
  }
  
  // Check parent environment
  if (e->par) {
    return lenv_get(e->par, k);
  }

  return lval_err("Unbound symbol: '%s'", k->sym);
}

void lenv_def(lenv* e, lval* k, lval* v) {
  while(e->par) {
    e = e->par;
  }
  
  lenv_put(e, k, v);
}

void lenv_put(lenv* e, lval* k, lval* v) {
  
  // See if the symbol exists
  for (int i = 0; i < e->count; i++) {
    if (streq(e->syms[i], k->sym)) {
      // Delete and replace
      lval_del(e->vals[i]);
      e->vals[i] = lval_copy(v);
      return;
    }
  }
  
  // Allocate space for new entry
  e->count++;
  e->syms = realloc(e->syms, sizeof(char*) * e->count);
  e->vals = realloc(e->vals, sizeof(lval*) * e->count);
  
  // Copy contents
  int i = e->count-1;
  e->syms[i] = malloc(strlen(k->sym)+1);
  strcpy(e->syms[i], k->sym);
  e->vals[i] = lval_copy(v);
}

void lenv_put_native(lenv* e, char* name, lnative func) {
  lval* k = lval_sym(name);
  lval* v = lval_fun(func);
  lenv_put(e, k, v);
  lval_del(k);
  lval_del(v);
}
