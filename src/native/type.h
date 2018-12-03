#ifndef TYPE_H
#define TYPE_H

typedef enum op_type {
  // Arithmatic
  ADD,
  SUB,
  MUL,
  DIV,
  MOD,
  POW,
  MIN,
  MAX,
  
  // Conditional
  GT,
  GE,
  LT,
  LE,
} op_type;

// double.c
lval* op_double(lenv* e, op_type type, double a, double b);

// long.c
lval* op_long(lenv* e, op_type type, long a, long b);

#endif
