#include "native.h"

//
// Math
//
lval* helper_op(lenv* e, lval* a, op_type type) {
  
  // Ensure all the arguments are numbers
  for (int i = 0; i < a->count; i++) {
    if (!lval_is_num(a->cell[i])) {
      lval_del(a);
      return lval_err(
        lenv_get_eval(e)->stack,
        "Cannot operate on non-number");
    }
  }
  
  // Pop the first element
  lval* x = lval_pop(a, 0);
  int xtype = x->type;
  
  // If there are no arguments and the operation is subtraction,
  // perform unary negation
  if (a->count == 0 && type == SUB) {
    switch (xtype) {
    
    case LVAL_LONG:
      ; // http://stackoverflow.com/questions/18496282/why-do-i-get-a-label-can-only-be-part-of-a-statement-and-a-declaration-is-not-a
      long l = -x->num_l;
      lval_del(a);
      lval_del(x);
      return lval_long(l);
      
    case LVAL_DOUBLE:
      ;
      double d = -x->num_d;
      lval_del(a);
      lval_del(x);
      return lval_double(d);
    
    default:
      lval_del(a);
      lval_del(x);
      return lval_err(
        lenv_get_eval(e)->stack,
        "Cannot perform unary negation on %s",
        lval_type_name(xtype));
    
    }
  }

  // Cumulatively apply arguments
  while (a->count > 0) {
  
    // Pop the next element
    lval* y = lval_pop(a, 0);
    
    int ytype = y->type;
    
    // Convert x to double if necessary
    if (xtype == LVAL_LONG && ytype == LVAL_DOUBLE) {
      lval* newx = lval_double((double) x->num_l);
      lval_del(x);
      x = newx;
      xtype = LVAL_DOUBLE;
    }
    
    // Convert y to double if necessary
    if (xtype == LVAL_DOUBLE && ytype == LVAL_LONG) {
      lval* newy = lval_double((double) y->num_l);
      lval_del(y);
      y = newy;
      ytype = LVAL_DOUBLE;
    }
    
    // Perform the operation per-type
    lval* step = NULL;
    switch (xtype) {
    
    case LVAL_LONG:
      step = op_long(e, type, x->num_l, y->num_l);
      break;
      
    case LVAL_DOUBLE:
      step = op_double(e, type, x->num_d, y->num_d);
      break;
      
    default:
      lval_del(x);
      lval_del(y);
      lval_del(a);
      return lval_err(
        lenv_get_eval(e)->stack,
        "Unsupported operation type: %s",
        lval_type_name(xtype));
    }
    
    // Replace old x value with the result of the operation
    lval_del(x);
    x = step;
    
    // Free the y value
    lval_del(y);
  }
 
  // Free the arguments list
  lval_del(a);

  return x;
}

lval* native_add(lenv* e, lval* a) {
  return helper_op(e, a, ADD);
}

lval* native_sub(lenv* e, lval* a) {
  return helper_op(e, a, SUB);
}

lval* native_mul(lenv* e, lval* a) {
  return helper_op(e, a, MUL);
}

lval* native_div(lenv* e, lval* a) {
  return helper_op(e, a, DIV);
}

lval* native_mod(lenv* e, lval* a) {
  return helper_op(e, a, MOD);
}

lval* native_pow(lenv* e, lval* a) {
  return helper_op(e, a, POW);
}

lval* native_min(lenv* e, lval* a) {
  return helper_op(e, a, MIN);
}

lval* native_max(lenv* e, lval* a) {
  return helper_op(e, a, MAX);
}

lval* native_gt(lenv* e, lval* a) {
  return helper_op(e, a, GT);
}

lval* native_ge(lenv* e, lval* a) {
  return helper_op(e, a, GE);
}

lval* native_lt(lenv* e, lval* a) {
  return helper_op(e, a, LT);
}

lval* native_le(lenv* e, lval* a) {
  return helper_op(e, a, LE);
}

/*
lval* native_gt(lenv* e, lval* a) {
  LASSERT_COND(a, ">");
  
  int r = a->cell[0]->num_l > a->cell[1]->num_l;
  lval_del(a);
  return lval_long(r);
}

lval* native_ge(lenv* e, lval* a) {
  LASSERT_COND(a, ">=");
  
  int r = a->cell[0]->num_l >= a->cell[1]->num_l;
  lval_del(a);
  return lval_long(r);
}

lval* native_lt(lenv* e, lval* a) {
  LASSERT_COND(a, "<");
  
  int r = a->cell[0]->num_l < a->cell[1]->num_l;
  lval_del(a);
  return lval_long(r);
}

lval* native_le(lenv* e, lval* a) {
  LASSERT_COND(a, "<=");
  
  int r = a->cell[0]->num_l <= a->cell[1]->num_l;
  lval_del(a);
  return lval_long(r);
}
*/
