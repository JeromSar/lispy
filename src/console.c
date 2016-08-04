// If we're compiling on Windows, compile these functions
#ifdef _WIN32
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char buffer[2048];

// Fake readline function
inline static char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

// Fake add_history function
// Windows already allows you to use command history by default
inline static void add_history(char* unused) {}

// We're compiling on Unix, include the editline headers
#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

char* console_readline(char* prompt) {
  return readline(prompt);
}

void console_add_history(char* history) {
  add_history(history);
}
