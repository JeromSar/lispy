#include "native.h"
#include "reader.h"
#include "lcontext.h"

//
// Core
//

// Helper for 'def' and '='
lval* helper_var(lenv* e, lval* a, char* func) {
  LASSERT_TYPE(a, func, 0, LVAL_QEXPR);
  
  // Ensure all of the elements are symbols
  lval* syms = a->cell[0];
  for (int i = 0; i < syms->count; i++) {
    LASSERT(a, syms->cell[i]->type == LVAL_SYM,
      "Function '%s' cannot define non-symbol. "
      "Got %s, Expected %s.",
      func,
      lval_type_name(syms->cell[i]->type),
      lval_type_name(LVAL_SYM));
  }
  
  LASSERT(a, (syms->count == a->count-1),
    "Function '%s' passed too many arguments for symbols. "
    "Got %i, Expected %i.",
    func,
    syms->count,
    a->count-1);
  
  // TODO: Remove debug
  printf("def: Printing stack -- ");
  stack_print(lenv_get_eval(e)->stack);
  
  for (int i = 0; i < syms->count; i++) {
    // def: define globally
    if (streq(func, "def")) {
      lenv_def(e, syms->cell[i], a->cell[i+1]);
    }
    
    // put: define locally
    if (streq(func, "=")) {
      lenv_put(e, syms->cell[i], a->cell[i+1]);
    }
  }
  
  lval_del(a);
  return lval_sexpr();
}

lval* native_def(lenv* e, lval* a) {
  return helper_var(e, a, "def");
}

lval* native_put(lenv* e, lval* a) {
  return helper_var(e, a, "=");
}

lval* native_isdef(lenv* e, lval* a) {
  LASSERT_ARGS(a, "isdef", 1);
  LASSERT_TYPE(a, "isdef", 0, LVAL_QEXPR);
  
  lval* arg = a->cell[0];
  LASSERT(a, arg->count == 1,
    "Function 'isdef' argument 0 is of wrong length. Got %i, Expected 1.",
    arg->count);

  LASSERT(a, arg->cell[0]->type == LVAL_SYM,
    "Function 'isdef' argument 0 contains wrong type. Got %s, Expected %s.",
    lval_type_name(arg->cell[0]->type),
    lval_type_name(LVAL_SYM));
    
  lval* v = lenv_get(e, arg->cell[0]);
  int r;
  if (v->type == LVAL_ERR) {
    r = 0;
  } else {
    r = 1;
  }
  
  lval_del(a);
  return lval_long(r);
}

lval* native_lambda(lenv* e, lval* a) {
  LASSERT_ARGS(a, "\\", 2);
  LASSERT_TYPE(a, "\\", 0, LVAL_QEXPR);
  LASSERT_TYPE(a, "\\", 1, LVAL_QEXPR);

  // Check arg 0 contains only symbols
  lval* formals = a->cell[0];
  for (int i = 0; i < formals->count; i++) {
    LASSERT(a, (formals->cell[i]->type == LVAL_SYM),
      "Function '\' Cannot define non-symbol. Got %s, Expected %s.",
      lval_type_name(formals->cell[i]->type),
      lval_type_name(LVAL_SYM));
  }
  
  lval* newformals = lval_pop(a, 0);
  lval* newbody = lval_pop(a, 0);
  lval_del(a);
  
  return lval_lambda(newformals, newbody);
}

lval* native_exit(lenv* e, lval* a) {
  LASSERT_ARGS(a, "exit", 1);
  LASSERT_TYPE(a, "exit", 0, LVAL_LONG);
  
  int code = a->cell[0]->num_l;
  lval_del(a);
  main_exit(code);
  return lval_sexpr();
}

lval* native_load(lenv* e, lval* a) {
  LASSERT_ARGS(a, "load", 1);
  LASSERT_TYPE(a, "load", 0, LVAL_STR);

  mpc_result_t r;
  if (!mpc_parse_contents(a->cell[0]->str, grammar_get(), &r)) {
    char* err_msg = mpc_err_string(r.error);
    mpc_err_delete(r.error);
    
    // Create new error
    lval* err = lval_err(
      lenv_get_eval(e)->stack,
      "Could not load file: %s, ", err_msg);
    free(err_msg);
    lval_del(a);
    
    // Print error
    lval_println(err);

    return err;
  }
  
  // Setup
  reader_set_symtable(lenv_get_eval(e)->symtable);
  reader_set_filename(a->cell[0]->str);
  
  // Read
  lval* expr = reader_read(r.output);
  //mpc_ast_print(r.output);
  mpc_ast_delete(r.output);
  
  // Get context
  lcontext* ctx = lenv_get_eval(e);
  
  // Evaluate each expression
  while (expr->count) {
    lval* pre = lval_pop(expr, 0);
    lval* post = lcontext_eval(ctx, pre);

    // Print errors
    if (post->type == LVAL_ERR) {
      lval_println(post);

      // Cleanup
      lval* error = lval_copy(post);
      lval_del(post);
      lval_del(expr);
      lval_del(a);
      return error;
    }
    
    lval_del(post);
  }
  
  // Cleanup
  lval_del(expr);
  lval_del(a);
  return lval_sexpr();
}

lval* native_show(lenv* e, lval* a) {
  for (int i = 0; i < a->count; i++) {
    lval_print(a->cell[i]); putchar(' ');
  }

  /* Print a newline and delete arguments */
  putchar('\n');
  lval_del(a);

  return lval_sexpr();
}

lval* native_print(lenv* e, lval* a) {
  LASSERT_ARGS(a, "print", 1);
  LASSERT_TYPE(a, "print", 0, LVAL_STR);
  
  puts(a->cell[0]->str);
  
  lval_del(a);
  return lval_sexpr();
}

lval* native_error(lenv* e, lval* a) {
  LASSERT_ARGS(a, "error", 1);
  LASSERT_TYPE(a, "error", 0, LVAL_STR);
  
  lval* err = lval_err(
    lenv_get_eval(e)->stack,
    a->cell[0]->str);
  
  lval_del(a);
  return err;
}

lval* native_try(lenv* e, lval* a) {
  LASSERT_ARGS(a, "try", 1);
  LASSERT_TYPE(a, "try", 0, LVAL_QEXPR);
  
  lval* x = lval_take(a, 0);

  // Make our expression a Q-expr
  x->type = LVAL_SEXPR;
  
  // Get eval environment
  lcontext* ctx = lenv_get_eval(e);

  lval* result = lcontext_eval(ctx, x);
  
  if (result->type == LVAL_ERR) {
    // Error, return the error string
    lval* message = lval_str(result->err);
    lval_del(result);
    return message;
  }
  
  // No error, return empty S-expression
  lval_del(result);
  return lval_sexpr();
}
