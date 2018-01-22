#include "lcontext.h"
#include "util.h" // streq
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

lval* lcontext_eval(lcontext* ctx, lval* v) {
  // Evaluate symbol
  if (v->type == LVAL_SYM) {
    lval* x = lenv_get(ctx->env, v);
    lval_del(v);
    return x;
  }

  // Evaluate S-expressions
  if (v->type == LVAL_SEXPR) {
    return lcontext_eval_sexpr(ctx, v);
  }
  
  // All other types remain the same
  return v;
}

lval* lcontext_eval_sexpr(lcontext* ctx, lval* v) {
  
  // Evaluate children
  for (int i = 0; i < v->count; i++) {
    v->cell[i] = lcontext_eval(ctx, v->cell[i]);
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
  
  // lcontext_call does cleanup
  return lcontext_call(ctx, f, v);
}

lval* lcontext_call(lcontext* ctx, lval* f, lval* a) {

  lval* rval;

  if (f->loc) {
    stack_push(ctx->stack, f->loc);
  } else {
    stack_push(ctx->stack, 0);
  }

  // If native, simply call
  if (f->native) {
    rval = f->native(ctx->env, a);
    
    if (rval->type == LVAL_ERR) {
      lcontext_handle_error(ctx, rval);
    }
    
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
        "Function passed too many arguments. "
        "Got %i, Expected %i.",
        given,
        total);

      lcontext_handle_error(ctx, rval);
      return rval;
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
      lenv_put(f->env, nsym, native_list(ctx->env, a));
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
        rval = lval_err("Function format invalid. "
          "Symbol '&' not followed by single symbol.");
        lcontext_handle_error(ctx, rval);
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
  f->env->par = ctx->env;
  
  // Eval and return
  rval = native_eval(
    f->env,
    lval_add(lval_sexpr() ,
    lval_copy(f->body)));

  if (rval->type == LVAL_ERR) {
    lcontext_handle_error(ctx, rval);
  }

  stack_pop(ctx->stack);
  return rval;  
}

void lcontext_handle_error(lcontext* ctx, lval* err) {
  if (err->type != LVAL_ERR) {
    printf("Error: Handling error of non-error type!");
    return;
  }
  
  // Print stack trace
  stack* st = ctx->stack;
  symtable* syms = ctx->symtable;
  
  lval_println(err);
  for (int i = 0; i < st->len; i++) {
    sym_loc* s = symtable_lookup_sym(syms, st->syms[i]);
    if (s != NULL) {
      printf("\tat %s, line %d, column %d\n",
        s->filename,
        s->row,
        s->col);
    } else {
      printf("\tat <unknown>, line ?, column ?\n");
    }
  }
}
