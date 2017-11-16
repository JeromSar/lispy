#include "native.h"

void native_addall(lenv* e) {
  // Core
  lenv_put_native(e, "def",    native_def);
  lenv_put_native(e, "=",      native_put);
  lenv_put_native(e, "isdef",  native_isdef);
  lenv_put_native(e, "exit",   native_exit);
  lenv_put_native(e, "\\",     native_lambda);
  lenv_put_native(e, "load",   native_load);
  lenv_put_native(e, "show",   native_show);
  lenv_put_native(e, "print",  native_print);
  lenv_put_native(e, "error",  native_error);
  lenv_put_native(e, "assert", native_assert);

  // Math
  lenv_put_native(e, "+",     native_add);
  lenv_put_native(e, "-",     native_sub);
  lenv_put_native(e, "*",     native_mul);
  lenv_put_native(e, "/",     native_div);
  lenv_put_native(e, "%",     native_mod);
  lenv_put_native(e, "^",     native_pow);
  lenv_put_native(e, "min",   native_min);
  lenv_put_native(e, "max",   native_max);
  
  // Conditional
  lenv_put_native(e, ">",     native_gt);
  lenv_put_native(e, ">=",    native_ge);
  lenv_put_native(e, "<",     native_lt);
  lenv_put_native(e, "<=",    native_le);
  lenv_put_native(e, "==",    native_eq);
  lenv_put_native(e, "!=",    native_ne);
  lenv_put_native(e, "||",    native_or);
  lenv_put_native(e, "&&",    native_and);
  lenv_put_native(e, "!",     native_not);
  lenv_put_native(e, "if",    native_if);

  // List
  lenv_put_native(e, "head",  native_head);
  lenv_put_native(e, "tail",  native_tail);
  lenv_put_native(e, "list",  native_list);
  lenv_put_native(e, "eval",  native_eval);
  lenv_put_native(e, "join",  native_join);
  lenv_put_native(e, "cons",  native_cons);
  lenv_put_native(e, "len",   native_len);
}

