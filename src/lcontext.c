#include "lcontext.h"
#include "util/util.h" // streq
#include "lval.h"
#include "native/native.h"
#include "lenv.h"
#include "symtable.h"
#include "stack.h"

lcontext* lcontext_new() {
  lcontext* l = malloc(sizeof(lcontext));
  l->stack = stack_new();
  l->symtable = symtable_new();
  l->env = lenv_new();

  // Set evaluation environment
  l->env->ctx = l;
  return l;
}

void lcontext_del(lcontext* l) {
  stack_del(l->stack);
  symtable_del(l->symtable);
  lenv_del(l->env);
  free(l);
}

lval* lcontext_eval(lcontext* ctx, lenv* e, lval* v) {
  // TODO: Remove debug: push stack
  stack_push_lval(ctx->stack, ctx, v);
  
  // Evaluate symbol
  if (v->type == LVAL_SYM) {
    lval* x = lenv_get(e, v);
    lval_del(v);

    stack_pop(ctx->stack);
    return x;
  }

  // Evaluate S-expressions
  if (v->type == LVAL_SEXPR) {
    stack_pop(ctx->stack);
    return lcontext_eval_sexpr(ctx, e, v);
  }
  
  // All other types remain the same
  stack_pop(ctx->stack);
  return v;
}

lval* lcontext_eval_sexpr(lcontext* ctx, lenv* e, lval* v) {
  stack_push_lval(ctx->stack, ctx, v);

  // Evaluate children
  for (int i = 0; i < v->count; i++) {
    v->cell[i] = lcontext_eval(ctx, e, v->cell[i]);
  }
  
  // Error checking
  for (int i =0; i < v->count; i++) {
    if (v->cell[i]->type == LVAL_ERR) {
      stack_pop(ctx->stack);
      return lval_take(v, i);
    }
  }
  
  // Empty expression
  if (v->count == 0) {
    stack_pop(ctx->stack);
    return v;
  }
  
  // Single expression
  if (v->count == 1) {
    stack_pop(ctx->stack);
    return lval_take(v, 0);
  }
  
  // Ensure first element is a symbol
  lval* f = lval_pop(v, 0);
  if (f->type != LVAL_FUN) {
    lval* err = lval_err(
      ctx->stack,
      "S-Expression starts with incorrect type. "
      "Got %s, Expected %s.",
      lval_type_name(f->type),
      lval_type_name(LVAL_FUN));
    lval_del(f);
    lval_del(v);
    stack_pop(ctx->stack);
    return err;
  }
  
  lval* ret = lcontext_call(ctx, e, f, v);

  // lcontext_call does cleanup
  stack_pop(ctx->stack);
  return ret;
}

lval* lcontext_call(lcontext* ctx, lenv* e, lval* f, lval* a) {
  // Push stack
  // TODO: is this right?
  stack_push_lval(ctx->stack, ctx, f);

  lval* rval;

  // If native, simply call
  if (f->native) {
    rval = f->native(e, a);    
    stack_pop(ctx->stack);
    return rval;
  }
  
  // Argument counts
  int given = a->count;
  int total = f->formals->count;
  
  // While arguments remain to be processed
  while(a->count) {
    
    // If we've ran out of formal arguments to bind
    if (f->formals->count == 0) {
      lval_del(a);
      rval = lval_err(
        ctx->stack,
        "Function passed too many arguments. "
        "Got %i, Expected %i.",
        given,
        total);

      stack_pop(ctx->stack);
      return rval;
    }

    lval* sym = lval_pop(f->formals, 0);

    // Ellipsis
    if (streq(sym->sym, "&")) {
      
      // Ensure this is the second to last symbol
      if (f->formals->count != 1) {
        lval_del(a);
        return lval_err(
          ctx->stack,
          "Function format invalid. "
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
      rval = lval_err(
        ctx->stack,
        "Function format invalid. "
        "Symbol '&' not followed by single symbol.");
      return rval;
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
    stack_pop(ctx->stack);
    return lval_copy(f);
  }

  // Set environment parent to evaluation environment;
  f->env->par = e;

  // Eval and return
  rval = native_eval(
    f->env,
    lval_add(lval_sexpr(),
      lval_copy(f->body)));

  stack_pop(ctx->stack);
  return rval;  
}
