#include "util.h" // streq
#include "eval.h"
#include "lval.h"
#include "native/native.h"
#include "lenv.h"
#include "stackof.h"

lval* lval_eval(lenv* e, lval* v) {

  if (stack_overflow()) {
    return lval_err("Stack overflow!");
  }

  // Evaluate symbol
  if (v->type == LVAL_SYM) {
    lval* x = lenv_get(e, v);
    lval_del(v);
    return x;
  }

  // Evaluate S-expressions
  if (v->type == LVAL_SEXPR) {
    return lval_eval_sexpr(e, v);
  }
  
  // All other types remain the same
  return v;
}

lval* lval_eval_sexpr(lenv* e, lval* v) {
  
  // Evaluate children
  for (int i = 0; i < v->count; i++) {
    v->cell[i] = lval_eval(e, v->cell[i]);
  }
  
  // Error checking
  for (int i =0; i < v->count; i++) {
    if (v->cell[i]->type == LVAL_ERR) {
      return lval_take(v, i);
    }
  }
  
  // Empty expression
  if (v->count == 0) {
    return v;
  }
  
  // Single expression
  if (v->count == 1) {
    return lval_take(v, 0);
  }
  
  // Ensure first element is a symbol
  lval* f = lval_pop(v, 0);
  if (f->type != LVAL_FUN) {
    lval* err = lval_err(
      "S-Expression starts with incorrect type. "
      "Got %s, Expected %s.",
      lval_type_name(f->type),
      lval_type_name(LVAL_FUN));
    lval_del(f);
    lval_del(v);
    return err;
  }
  
  // lval_call does cleanup
  return lval_call(e, f, v);
}

lval* lval_call(lenv* e, lval* f, lval* a) {
  
  // If native, simply call
  if (f->native) {
    return f->native(e, a);
  }
  
  // Argument counts
  int given = a->count;
  int total = f->formals->count;
  
  // While arguments remain to be processed
  while(a->count) {
    
    // If we've ran out of formal arguments to bind
    if (f->formals->count == 0) {
      lval_del(a);
      return lval_err(
        "Function passed too many arguments. "
        "Got %i, Expected %i.",
        given,
        total);
    }
    
    
    lval* sym = lval_pop(f->formals, 0);
    
    // Ellipsis
    if (streq(sym->sym, "&")) {
      
      // Ensure this is the second to last symbol
      if (f->formals->count != 1) {
        lval_del(a);
        return lval_err("Function format invalid. "
          "Symbol '&' not followed by single symbol.");
      }
      
      // Next formal should be bound to remaining arguments
      lval* nsym = lval_pop(f->formals, 0);
      lenv_put(f->env, nsym, native_list(e, a));
      lval_del(sym);
      lval_del(nsym);
      break;
    }
    
    // Put argument in the environment
    lval* val = lval_pop(a, 0);
    lenv_put(f->env, sym, val);
    
    // Cleanup
    lval_del(sym);
    lval_del(val);
  }
  
  // Arguments are now bound, can be cleaned
  lval_del(a);
  
  // If '&' remains in formal list, bind to empty list
  if (f->formals->count > 0 &&
    streq(f->formals->cell[0]->sym, "&")) {
      // Check that & is not passed invalidly
      
      if (f->formals->count != 2) {
        return lval_err("Function format invalid. "
          "Symbol '&' not followed by single symbol.");
      }
      
      // Pop and delete '&'
      lval_del(lval_pop(f->formals, 0));
      
      // Pop next symbol, create empty list
      lval* sym = lval_pop(f->formals, 0);
      lval* val = lval_qexpr();
      
      // Bind to environment and delete
      lenv_put(f->env, sym, val);
      lval_del(sym);
      lval_del(val);
    }
  
  // Partially evaulated?
  if (f->formals->count != 0) {
    return lval_copy(f);
  }
  
  // Set environment parent to evaluation environment;
  f->env->par = e;
  
  // Eval and return
  return native_eval(
    f->env, lval_add(lval_sexpr() , lval_copy(f->body)));
}
