#include "native.h"

// Operations on double-type
lval* op_double(lenv* e, op_type type, double a, double b) {
  
  switch (type) {
  
    case ADD:
      return lval_double(a + b);
      
    case SUB:
      return lval_double(a - b);
      
    case MUL:
      return lval_double(a * b);
      
    case DIV:
      if (b == 0) {
        return lval_err(
          lenv_get_eval(e)->stack,
          "Division by zero");
      }
      return lval_double(a / b);

    // TODO: Stringify
    case MOD:
      return lval_err(
        lenv_get_eval(e)->stack,
        "Cannot perform '%i' operation on double type.", type);
      
    case POW:
      return lval_err(
        lenv_get_eval(e)->stack,
        "Cannot perform '%i' operation on double type.", type);
      
    case MIN:
      return lval_double(a < b ? a : b);
      
    case MAX:
      return lval_double(a > b ? a : b);
    
    case GT:
      return lval_long(a > b ? 1 : 0);
    
    case GE:
      return lval_long(a >= b ? 1 : 0);
    
    case LT:
      return lval_long(a < b ? 1 : 0);
    
    case LE:
      return lval_long(a <= b ? 1 : 0);

    default:
      return lval_err(
        lenv_get_eval(e)->stack,
        "No double operation: %i", (int) type);
  }
}