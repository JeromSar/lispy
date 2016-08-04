#include "native.h"

// Operations on long-type
lval* op_long(op_type type, long a, long b) {
  
  switch (type) {
  
    case ADD:
      return lval_long(a + b);
      
    case SUB:
      return lval_long(a - b);
      
    case MUL:
      return lval_long(a * b);
      
    case DIV:
      if (b == 0) {
        return lval_err("Division by zero");
      }
      return lval_long(a / b);
      
    case MOD:
      if (b == 0) {
        return lval_err("Division by zero");
      }
      return lval_long(a % b);
      
    case POW:
      return lval_long(lpow(a, b));
      
    case MIN:
      return lval_long(a < b ? a : b);
      
    case MAX:
      return lval_long(a > b ? a : b);
    
    case GT:
      return lval_long(a > b ? 1 : 0);
    
    case GE:
      return lval_long(a >= b ? 1 : 0);
    
    case LT:
      return lval_long(a < b ? 1 : 0);
    
    case LE:
      return lval_long(a <= b ? 1 : 0);

    default:
      return lval_err("No long operation: %i", (int) type);
  }
}