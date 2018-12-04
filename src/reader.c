#include "lval.h"
#include "mpc/mpc.h"
#include "util/util.h" // streq
#include "lispy.h" // main_exit
#include "reader.h"

#define UNKNOWN_FILENAME "<unknown>"

static char* reader_filename = UNKNOWN_FILENAME;
static symtable* reader_symtable = NULL;

static int create_sym_loc(mpc_ast_t* t) {
  if (reader_symtable == NULL) {
    return 0;
  }
  
  mpc_state_t state = t->state;

  return symtable_push_sym(
    reader_symtable,
    reader_filename,
    state.row + 1,
    state.col + 1
  );
}

static lval* reader_read_long(mpc_ast_t* t) {
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  if (errno == ERANGE || errno == EINVAL) {
    return lval_err(NULL, "Invalid long number: %s", t->contents);
  }
  
  lval* r = lval_long(x);
  r->loc = create_sym_loc(t);
  return r;
}

static lval* reader_read_double(mpc_ast_t* t) {
  errno = 0;
  double x = strtod(t->contents, NULL);
  if (errno == ERANGE || errno == EINVAL) {
    return lval_err(NULL, "Invalid double number: %s", t->contents);
  }
  
  lval* r = lval_double(x);
  return r;
}

static lval* reader_read_str(mpc_ast_t* t) {
  // Cut off final quote character
  t->contents[strlen(t->contents)-1] = '\0';
  
  // Make a copy, skipping the first quote
  char* unescaped = malloc(strlen(t->contents+1) + 1);
  strcpy(unescaped, t->contents+1);
  unescaped = mpcf_unescape(unescaped);
  lval* str = lval_str(unescaped);
  free(unescaped);
  return str;
}

void reader_set_filename(char* filename) {
  if (!streq(reader_filename, UNKNOWN_FILENAME)) {
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

lval* reader_read(mpc_ast_t* t) {
  
  // Numbers, strings and symbols
  lval* x = NULL;
  if (strstr(t->tag, "double"))       { x = reader_read_double(t); }
  else if (strstr(t->tag, "long"))    { x = reader_read_long(t); }
  else if (strstr(t->tag, "string"))  { x = reader_read_str(t); }
  else if (strstr(t->tag, "symbol"))  { x = lval_sym(t->contents); }
  
  if (x) {
    x->loc = create_sym_loc(t);
    return x;
  }
  
  // If root (>), S-expr or Q-expr, then create an empty list
  if (streq(t->tag, ">"))           { x = lval_sexpr(); }
  else if (strstr(t->tag, "sexpr")) { x = lval_sexpr(); }
  else if (strstr(t->tag, "qexpr")) { x = lval_qexpr(); }
  
  if (!x) {
    // TODO: Convert to error?
    printf("Could not read node, unrecognized tag: '%s'", t->tag);
    main_exit(1);
  }
  
  // Fill the list with valid expressions
  for (int i = 0; i < t->children_num; i++) {
    if (strcmp(t->children[i]->contents, "(") == 0) { continue; }
    if (strcmp(t->children[i]->contents, ")") == 0) { continue; }
    if (strcmp(t->children[i]->contents, "}") == 0) { continue; }
    if (strcmp(t->children[i]->contents, "{") == 0) { continue; }
    if (strcmp(t->children[i]->tag,  "regex") == 0) { continue; }
    if (strstr(t->children[i]->tag, "comment"))     { continue; }
    x = lval_add(x, reader_read(t->children[i]));
  }

  x->loc = create_sym_loc(t);
  return x;
}

int ast_count_nodes(mpc_ast_t* t) {
  if (t->children_num == 0) { return 1; }
  if (t->children_num >= 1) {
    int total = 1;
    for (int i = 0; i < t->children_num; i++) {
      total += ast_count_nodes(t->children[i]);
    }
    return total;
  }
  return 0;
}

int ast_count_leaves(mpc_ast_t* t) {
  if (t->children_num == 0) { return 1; }
  if (t->children_num >= 1) {
    int total = 0;
    for (int i = 0; i < t->children_num; i++) {
      total += ast_count_leaves(t->children[i]);
    }
    return total;
  }
  return 0;
}

int ast_count_branches(mpc_ast_t* t) {
  if (t->children_num == 0) { return 0; }
  if (t->children_num >= 1) {
    int total = 1;
    for (int i = 0; i < t->children_num; i++) {
      total += ast_count_branches(t->children[i]);
    }
    return total;
  }
  return 0;
}

