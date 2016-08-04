#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "mpc/mpc.h"

#define GRAMMAR "                                              \
                                                               \
      double   : /-?[0-9]+\\.[0-9]+/ ;                         \
      long     : /-?[0-9]+/ ;                                  \
      symbol   : /[a-zA-Z0-9_+\\-*\\/%\\^\\\\=<>!&$\\|\\:]+/ ; \
      string   : /\"(\\\\.|[^\"])*\"/ ;                        \
      comment  : /;[^\\r\\n]*/ ;                               \
      sexpr    : '(' <expr>* ')' ;                             \
      qexpr    : '{' <expr>* '}' ;                             \
      expr     : <double> | <long> | <symbol> | <string>       \
               | <comment> | <sexpr>  | <qexpr> ;              \
      lispy    : /^/ <expr>* /$/ ;                             \
                                                               \
   "

mpc_parser_t* grammar_create(void);
mpc_parser_t* grammar_get(void);
void grammar_cleanup(void);

#endif
