#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "lispy.h"
#include "util.h"
#include "console.h"
#include "grammar.h"
#include "lval.h"
#include "lcontext.h"
#include "lenv.h"
#include "native/native.h"
#include "reader.h"
#include "mpc/mpc.h"

static mpc_parser_t* parser;
static lcontext* ctx;

void main_signal(int signal) {
  puts("Exit.\n");
  main_exit(1);
}

void main_exit(int code) {
  if (parser) {
    grammar_cleanup();
  }
  
  if (ctx) {
    lcontext_del(ctx);
  }
  exit(code);
}

#ifndef BUILDING_TESTS
int main(int argc, char** argv) {
  // Setup signals
  signal(SIGINT, main_signal);
  signal(SIGTERM, main_signal);

  // Create parser
  parser = grammar_create();
   
  // Create evaluation context
  ctx = lcontext_new();
  native_addall(ctx->env);
  
  // Setup reader
  reader_set_symtable(ctx->symtable);

  // A list of files is supplied
  if (argc > 1) {
    for(int i = 1; i < argc; i++) {
      
      // Make arguments
      lval* args = lval_add(lval_sexpr(), lval_str(argv[i]));

      lval* x = native_load(ctx->env, args);

      // Print errors
      if (x->type == LVAL_ERR) {
        lval_println(x);
        lval_del(x);
        main_exit(1);
      }
    }

    main_exit(0);
  }

  // Interactive mode
  reader_set_filename("<stdin>");
  puts("Lispy version 0.0.1");
  puts("Press Ctrl+c to exit\n");

  while (1) {
    
    char* input = console_readline("lispy> ");
    console_add_history(input);
    
    // Attempt to parse the user input
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, parser, &r)) {
      
      mpc_ast_t* ast = r.output;
      
      /*
      mpc_ast_print(ast);
      printf("Number of nodes: %i\n", ast_count_nodes(ast));
      printf("Number of leaves: %i\n", ast_count_leaves(ast));
      printf("Number of branches: %i\n", ast_count_branches(ast));
      putchar('\n');
      */
      
      // Read lval
      lval* x = reader_read(ast);

      // Evaluate
      x = lcontext_eval(ctx, x);

      // Print
      lval_println(x);
      
      // Cleanup
      lval_del(x);
      mpc_ast_delete(ast);
    } else {
      // Failure: Print the error
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    
    puts("");
    free(input);
  }
  
  
  main_exit(0);
}
#endif
