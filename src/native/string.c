#include "native.h"

//
// String utilities
//
lval* native_str(lenv* e, lval* a) {
  LASSERT_ARGS(a, "str", 1);

  lval* par = lval_pop(a, 0);
  lval_del(a);

  lval* x;

  // TODO: switch to a single stringify method for core/show and string/str.
  switch (par->type) {
    case LVAL_DOUBLE:
      // Default to precision of 15 characters
      // TODO: Optional precision argument?
      ;
      char f_double[16];
      snprintf(f_double, 16, "%f", par->num_d);
      x = lval_str(f_double);
      break;
    case LVAL_LONG:
      // Default to precision of 15 characters
      // TODO: Optional precision argument?
      ;
      char f_long[16];
      snprintf(f_long, 16, "%ld", par->num_l);
      x = lval_str(f_long);
      break;
    case LVAL_ERR:
      x = par;
      break;
    case LVAL_SYM:
      x = lval_str(par->sym);
      break;
    case LVAL_STR:
      x = par;
      break;
    case LVAL_FUN:
      if (par->native) {
        x = lval_str("<native>");
      } else {
        // TODO: Print internals?
        x = lval_str("<function>");
      }
      break;
    case LVAL_SEXPR:
      x = lval_str("<s-expr>");
      break;
    case LVAL_QEXPR:
      x = lval_str("<q-expr>");
      break;
    default:
      x = lval_err(
        lenv_get_eval(e)->stack,
        strcat("Conversion not supported for type: ",
        lval_type_name(par->type)));
      break;
  }

  if (x != par) {
    lval_del(par);
  }
  return x;
}
