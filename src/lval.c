#include <stdlib.h> // strtol
#include <stdio.h>
#include <stdarg.h>

#include "lval.h"
#include "util.h" // streq

//
// Lval heap constructors
//

// Create a new number (long) type lval
lval* lval_long(long x) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_LONG;
  v->num_l = x;
  return v;
}

// Create a new number (double) type lval
lval* lval_double(double x) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_DOUBLE;
  v->num_d = x;
  return v;
}

// Create a new error type lval
lval* lval_err(char* fmt, ...) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_ERR;
  
  // Create a va list and init it
  va_list va;
  va_start(va, fmt);
  
  // Allocate 512 bytes of space
  v->err = malloc(512);
  
  // printf the error string with a maximum of 511 chars
  vsnprintf(v->err, 511, fmt, va);
  
  // Reallocate to the number of bytes we actually used
  v->err = realloc(v->err, strlen(v->err) + 1);
  
  // Cleanup va list
  va_end(va);
  
  return v;
}

// Create a new symbol type lval
lval* lval_sym(char* s) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_SYM;
  v->sym = malloc(strlen(s) + 1);
  strcpy(v->sym, s);
  return v;
}

// Create a new string type lval
lval* lval_str(char* s) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_STR;
  v->str = malloc(strlen(s) + 1);
  strcpy(v->str, s);
  return v;
}

// Create a new lval referring to a native function
lval* lval_fun(lnative func) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_FUN;
  v->native = func;
  return v;
}

// Create a new lval lambda with the specified formals and body
lval* lval_lambda(lval* formals, lval* body) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_FUN;
  v->native = NULL;
  v->env = lenv_new();
  v->formals = formals;
  v->body = body;
  return v;
}

// Create a new (and empty) S-expression type lval
lval* lval_sexpr(void) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_SEXPR;
  v->count = 0;
  v->cell = NULL;
  return v;
}

// Create a new (and empty) Q-expression type lval
lval* lval_qexpr(void) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_QEXPR;
  v->count = 0;
  v->cell = NULL;
  return v;
}

//
// Operations
//

void lval_del(lval* v) {
  switch (v->type) {
    // Don't do anyting for numbers
    case LVAL_LONG:
    case LVAL_DOUBLE:
      break;
      
    case LVAL_ERR:
      free(v->err);
      break;
      
    case LVAL_SYM:
      free(v->sym);
      break;
      
    case LVAL_STR:
      free(v->str);
      break;
      
    case LVAL_FUN:
      if (!v->native) {
        lenv_del(v->env);
        lval_del(v->formals);
        lval_del(v->body);
      }
      break;
     
    // Delete all elements in the S-expression cells
    case LVAL_SEXPR:
    case LVAL_QEXPR:
      for (int i = 0; i < v->count; i++) {
        lval_del(v->cell[i]);
      }
      free(v->cell);
      break;
  }
  
  // Free the memory allocated for the lval struct itself
  free(v);
}

// Creates a value-equal duplicate of the lval
lval* lval_copy(lval* v) {
  lval* x = malloc(sizeof(lval));
  x->type = v->type;
  
  switch (v->type) {
    
    // Copy numbers and functions directly
    case LVAL_LONG: x->num_l = v->num_l; break;
    case LVAL_DOUBLE: x->num_d = v->num_d; break;
    
    // Copy strings using malloc and strcopy
    case LVAL_ERR:
      x->err = malloc(strlen(v->err) + 1);
      strcpy(x->err, v->err);
      break;
    case LVAL_SYM:
      x->sym = malloc(strlen(v->sym) + 1);
      strcpy(x->sym, v->sym);
      break;
    case LVAL_STR:
      x->str = malloc(strlen(v->str) + 1);
      strcpy(x->str, v->str);
      break;
      
    // Copy functions
    case LVAL_FUN:
      if (v->native) {
        x->native = v->native;
      } else {
        x->native = NULL;
        x->env = lenv_copy(v->env);
        x->formals = lval_copy(v->formals);
        x->body = lval_copy(v->body);
      }
      break;
    
    // Copy lists by sub-expression
    case LVAL_SEXPR:
    case LVAL_QEXPR:
      x->count = v->count;
      x->cell = malloc(sizeof(lval*) * x->count);
      for (int i = 0; i< x->count; i++) {
        x->cell[i] = lval_copy(v->cell[i]);
      }
      break;
  }
  
  return x;
}

// Returns 1 if the arguments are value-equal, 0 otherwise
int lval_eq(lval* x, lval* y) {
  if (x->type != y->type) {
    return 0;
  }

  switch (x->type) {
    
    case LVAL_LONG:
      return x->num_l == y->num_l;
    
    case LVAL_DOUBLE:
      return x->num_d == y-> num_d;
    
    case LVAL_ERR:
      return streq(x->err, y->err);
    
    case LVAL_SYM:
      return streq(x->sym, y->sym);
      
    case LVAL_STR:
      return streq(x->str, y->str);
    
    case LVAL_FUN:
      if (x->native) {
        return x->native == y->native;
      } else {
        if (!lval_eq(x->formals, y->formals)) {
          return 0;
        } else if (!lval_eq(x->body, y->body)) {
          return 0;
        } else {
          return 1;
        }
      }
      
    case LVAL_QEXPR:
    case LVAL_SEXPR:
      if (x->count != y->count) {
        return 0;
      }
      for (int i = 0; i < x->count; i++) {
        if (!lval_eq(x->cell[i], y->cell[i])) {
          return 0;
        }
      }
      return 1;
  }
  return 0;
}

// Returns
int lval_is_num(lval* x) {
  switch (x->type) {
    case LVAL_LONG:
    case LVAL_DOUBLE:
      return 1;
    default:
      return 0;
  }
}

//
// AST -> lval operations
//

lval* lval_read_long(mpc_ast_t* t) {
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  if (errno == ERANGE || errno == EINVAL) {
    return lval_err("Invalid long number: %s", t->contents);
  }
  return lval_long(x);
}

lval* lval_read_double(mpc_ast_t* t) {
  errno = 0;
  double x = strtod(t->contents, NULL);
  if (errno == ERANGE || errno == EINVAL) {
    return lval_err("Invalid double number: %s", t->contents);
  }
  return lval_double(x);
}

lval* lval_read_str(mpc_ast_t* t) {
  // Cut off final quote character
  t->contents[strlen(t->contents)-1] = '\0';
  // Make a copy, missing out the first qu
  char* unescaped = malloc(strlen(t->contents+1) + 1);
  strcpy(unescaped, t->contents+1);
  unescaped = mpcf_unescape(unescaped);
  lval* str = lval_str(unescaped);
  free(unescaped);
  return str;
}

lval* lval_read(mpc_ast_t* t) {
  
  // Numbers, strings and symbols
  if (strstr(t->tag, "double")) { return lval_read_double(t); }
  if (strstr(t->tag, "long")) { return lval_read_long(t); }
  if (strstr(t->tag, "string")) { return lval_read_str(t); }
  if (strstr(t->tag, "symbol")) { return lval_sym(t->contents); }
  
  // If root (>), S-expr or Q-expr, then create an empty list
  lval* x = NULL;
  if (streq(t->tag, ">"))      { x = lval_sexpr(); }
  if (strstr(t->tag, "sexpr")) { x = lval_sexpr(); }
  if (strstr(t->tag, "qexpr")) { x = lval_qexpr(); }
  
  // Fill the list with valid expressions
  for (int i = 0; i < t->children_num; i++) {
    if (strcmp(t->children[i]->contents, "(") == 0) { continue; }
    if (strcmp(t->children[i]->contents, ")") == 0) { continue; }
    if (strcmp(t->children[i]->contents, "}") == 0) { continue; }
    if (strcmp(t->children[i]->contents, "{") == 0) { continue; }
    if (strcmp(t->children[i]->tag,  "regex") == 0) { continue; }
    if (strstr(t->children[i]->tag, "comment"))     { continue; }
    x = lval_add(x, lval_read(t->children[i]));
  }
  
  return x;
}

//
// S/Q-Expression operations
//

lval* lval_add(lval* v, lval* x) {
  v->count++;
  v->cell = realloc(v->cell, sizeof(lval*) * v->count);
  v->cell[v->count-1] = x;
  return v;
}

lval* lval_pop(lval* v, int i) {
  // Find the item at index 'i'
  lval* x = v->cell[i];
  
  // Shift memory after the item at 'i' over the top
  memmove(
    &v->cell[i],
    &v->cell[i+1],
    sizeof(lval*) * (v->count-i-1)
  );
  
  // Decrease the amount of items in the list
  v->count--;
  
  // Reallocate the used memory
  v->cell = realloc(v->cell, sizeof(lval*) * v->count);
  
  return x;
}

lval* lval_take(lval* v, int i) {
  lval* x = lval_pop(v, i);
  lval_del(v);
  return x;
}

lval* lval_join(lval* x, lval* y) {
  
  // For each cell in y, add it to x
  while (y->count) {
    x = lval_add(x, lval_pop(y, 0));
  }
  
  // Delete the empty y and return x
  lval_del(y);
  return x;
}

//
// Printing operations
//

// Returns a string representation of the lval type
char* lval_type_name(int type) {
  switch(type) {
    case LVAL_FUN:    return "Function";
    case LVAL_LONG:   return "Long";
    case LVAL_DOUBLE: return "Double";
    case LVAL_ERR:    return "Error";
    case LVAL_SYM:    return "Symbol";
    case LVAL_STR:    return "String";
    case LVAL_SEXPR:  return "S-Expression";
    case LVAL_QEXPR:  return "Q-Expression";
    default:          return "Unknown";
  }
}

// Prints an expression type lval
void lval_expr_print(lval* v, char open, char close) {
  putchar(open);
  for (int i = 0; i < v->count; i++) {
    // Print contained value
    lval_print(v->cell[i]);
    
    // Don't print the trailing space if this is the last element
    if (i != (v->count-1)) {
      putchar(' ');
    }
  }
  putchar(close);
}

void lval_str_print(lval* v) {
  // Make a copy
  char* escaped = malloc(strlen(v->str) + 1);
  strcpy(escaped, v->str);
  
  // Pass through mpc
  escaped = mpcf_escape(escaped);
  printf("\"%s\"", escaped);
  
  // Free
  free(escaped);
}

// Prints a lval
void lval_print(lval* v) {
  switch (v->type) {
    // Number
    case LVAL_LONG:
      printf("%ld", v->num_l);
      break;
      
    case LVAL_DOUBLE:
      printf("%f", v->num_d);
      break;

    // Error
    case LVAL_ERR:
      printf("Error: %s", v->err);
      break;
      
    // Symbol
    case LVAL_SYM:
      printf("%s", v->sym);
      break;
      
    case LVAL_STR:
      lval_str_print(v);
      break;
     
    // Function
    case LVAL_FUN:
      if (v->native) {
        printf("<native>");
      } else {
        printf("(\\ ");
        lval_print(v->formals);
        putchar(' ');
        lval_print(v->body);
        putchar(')');
      }
      break;
      
    // Expressions
    case LVAL_SEXPR:
      lval_expr_print(v, '(', ')');
      break;
    case LVAL_QEXPR:
      lval_expr_print(v, '{', '}');
      break;

    // Unknown value type
    default:
      printf("Error: Value type can not be printed: %d", v->type);
      break;
  }
}

void lval_println(lval* v) {
  lval_print(v);
  putchar('\n');
}

