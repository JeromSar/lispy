#include "grammar.h"
#include "mpc/mpc.h"

static mpc_parser_t* number_d;
static mpc_parser_t* number_l;
static mpc_parser_t* symbol;
static mpc_parser_t* string;
static mpc_parser_t* comment;
static mpc_parser_t* sexpr;
static mpc_parser_t* qexpr;
static mpc_parser_t* expr;
static mpc_parser_t* lispy;

mpc_parser_t* grammar_create() {
  number_d = mpc_new("double");
  number_l = mpc_new("long");
  symbol = mpc_new("symbol");
  string = mpc_new("string");
  comment = mpc_new("comment");
  sexpr = mpc_new("sexpr");
  qexpr = mpc_new("qexpr");
  expr = mpc_new("expr");
  lispy = mpc_new("lispy");
    
  mpca_lang(MPCA_LANG_DEFAULT, GRAMMAR,  
    number_d, number_l, symbol, string, comment,
    sexpr, qexpr, expr, lispy);
    
  return lispy;
}

mpc_parser_t* grammar_get() {
  return lispy;
}

void grammar_cleanup() {
  mpc_cleanup(9,
    number_d, number_l, symbol, string, comment,
    sexpr, qexpr, expr, lispy);
}