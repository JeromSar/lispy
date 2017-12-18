#include "lval.h"
#include "mpc/mpc.h"
#include "util.h" // streq

static const char* UNKNOWN_FILENAME = "<unknown>";

static char* reader_filename = UNKNOWN_FILENAME;
static symtable* reader_symtable = NULL;

static int create_sym_loc(mpc_ast_t* t) {
  if (reader_set_symtable == NULL) {
    return 0;
  }
  
  mpc_state_t state = ast->state;
  
  return symtable_push_sym(
    reader_symtable,
    "",
    state.row + 1,
    state.col + 1
  );
}

void reader_set_filename(char* filename) {
  if (reader_filename != UNKNOWN_FILENAME) {
    free(reader_filename);
  }
  
  if (filename == NULL) {
    reader_filename = UNKNOWN_FILENAME;
  } else {
    reader_filename = strdup(filename);
  }
}

void reader_set_symtable(symtable* st) {
  reader_symtable = st;
}

lval* reader_read_long(mpc_ast_t* t) {
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  if (errno == ERANGE || errno == EINVAL) {
    return lval_err("Invalid long number: %s", t->contents);
  }
  
  lval* r = lval_long(x);
  r->loc = lval_loc_ast(t);
  return r;
}

lval* lval_read_double(mpc_ast_t* t) {
  errno = 0;
  double x = strtod(t->contents, NULL);
  if (errno == ERANGE || errno == EINVAL) {
    return lval_err("Invalid double number: %s", t->contents);
  }
  
  lval* r = lval_double(x);
  r->loc = lval_loc_ast(t);
  return r;
}

lval* reader_read_str(mpc_ast_t* t) {
  // Cut off final quote character
  t->contents[strlen(t->contents)-1] = '\0';
  
  // Make a copy, skipping the first quote
  char* unescaped = malloc(strlen(t->contents+1) + 1);
  strcpy(unescaped, t->contents+1);
  unescaped = mpcf_unescape(unescaped);
  lval* str = lval_str(unescaped);
  free(unescaped);
  
  str->loc = lval_loc_ast(t);
  return str;
}

lval* reader_read(mpc_ast_t* t) {
  
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
  
  x->loc = lval_loc_ast(t);
  return x;
}
