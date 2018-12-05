#include "native.h"

//
// String utilities
//
lval* native_str(lenv* e, lval* a) {
  LASSERT_ARGS(a, "str", 1);

  // Pop first arg
  lval* par = lval_pop(a, 0);

  // Print to buffer
  buffer* buf = buffer_new(16);
  lval_print(buf, par);
  buffer_shrink(buf);

  // Return val
  lval* x = lval_str(buf->buf);

  // Cleanup
  buffer_del(buf);
  lval_del(par);
  lval_del(a);
  return x;
}
