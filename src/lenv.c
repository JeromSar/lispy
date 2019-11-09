#include "lenv.h"
#include "lval.h"
#include "util/util.h" // streq
#include "stack.h"

lenv* lenv_new(void) {
  lenv* e = malloc(sizeof(lenv));
  e->ctx = NULL;
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

  return lval_err(
    lenv_get_eval(e)->stack,
    "Unbound symbol: '%s'",
    k->sym);
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
  
  // TODO: Should this be /here/?
  // Add new symbol
  lcontext* ctx = lenv_get_eval(e);
  char* fullref = malloc((50 + strlen(name)) * sizeof(char));
  fullref[0] = '\0';
  strcat(fullref, "<internal:");
  strcat(fullref, name);
  strcat(fullref, ">");
  int key = symtable_push_sym(ctx->symtable, fullref, 0, 0);
  free(fullref);
  
  lval* k = lval_sym(name);
  lval* v = lval_fun(func);
  v->loc = key;
  
  lenv_put(e, k, v);
  lval_del(k);
  lval_del(v);
}

lcontext* lenv_get_eval(lenv* e) {
  lenv* c = e;
  while (c->par != NULL) {
    c = c->par;
  }
  return c->ctx;
}

void lenv_print(lenv* env, lcontext* ctx) {
  symtable* st = ctx->symtable;
  
  for (int i = 0; i < env->count; i++) {
    char* sym = env->syms[i];
    lval* val = env->vals[i];
    
    sym_loc* loc = symtable_lookup_sym(st, val->loc);
    if (loc == NULL) {
      printf("ctx: %s, defined at <unknown> (key: %d)\n", sym, val->loc);
    } else {
      printf("ctx: %s, defined at %s:%d:%d\n",
        sym,
        loc->filename,
        loc->row,
        loc->col);
    } 
  }  
}
