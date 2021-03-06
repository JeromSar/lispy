#ifndef LVAL_H
#define LVAL_H

#include "mpc/mpc.h"

// Some typedefs
// lenv type is defined here to avoid cyclic dependency
typedef struct lval lval;
typedef struct lenv lenv;
typedef lval* (*lnative)(lenv*, lval*);

// Only include lenv here, after typedefs
#include "lenv.h"

struct lenv {
  lenv* par;
  int count;
  char** syms;
  lval** vals;
};

struct lval {
  // Type of the lval
  int type;
  
  union {
    // LVAL_DOUBLE: Double
    double num_d;
    
    // LVAL_LONG: Long
    long num_l;

    // LVAL_ERR: Error message
    struct {
      char* err;
    };
      
    // LVAL_SYM: Symbol
    char* sym;
    
    // LVAL_STR: String
    char* str;
    
    // LVAL_FUN: Function pointer
    struct {
      lnative native;
      lenv* env;
      lval* formals;
      lval* body;
    };
    
    // LVAL_SEXPR, LVAL_QEXPR: Cells and cell-count
    struct {
      int count;
      struct lval** cell;
    };
  };
};

// Possible lval.type values
enum { LVAL_DOUBLE, LVAL_LONG, LVAL_ERR,   LVAL_SYM,
       LVAL_STR,    LVAL_FUN,  LVAL_SEXPR, LVAL_QEXPR };
       
// Heap constructors
lval* lval_long(long x);
lval* lval_double(double x);
lval* lval_err(char* fmt, ...);
lval* lval_sym(char* s);
lval* lval_str(char* str);
lval* lval_fun(lnative func);
lval* lval_lambda(lval* formals, lval* body);
lval* lval_sexpr(void);
lval* lval_qexpr(void);

// Operations
void lval_del(lval* v);
lval* lval_copy(lval* v);
int lval_eq(lval* x, lval* y);
int lval_is_num(lval* x);

// AST to lval reading
lval* lval_read_long(mpc_ast_t* t);
lval* lval_read_double(mpc_ast_t* t);
lval* lval_read_str(mpc_ast_t* t);
lval* lval_read(mpc_ast_t* t);

// S-expression operations
lval* lval_add(lval* v, lval* x);
lval* lval_pop(lval* v, int i);
lval* lval_take(lval* v, int i);
lval* lval_join(lval* x, lval* y);

// Print operations
char* lval_type_name(int t);
void lval_expr_print(lval* v, char open, char close);
void lval_str_print(lval* v);
void lval_print(lval* v);
void lval_println(lval* v);

#endif
