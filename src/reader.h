#ifndef READER_H
#define READER_H

#include "mpc/mpc.h"
#include "lval.h"
#include "symtable.h"

void reader_set_symtable(symtable* st);
lval* reader_read_long(mpc_ast_t* t);
lval* reader_read_double(mpc_ast_t* t);
lval* reader_read_str(mpc_ast_t* t);
lval* reader_read(mpc_ast_t* t);

#endif
