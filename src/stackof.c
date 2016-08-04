#include <stdlib.h>
#include "stackof.h"

static char* first_stack = NULL;

int stack_overflow() {

  // Stack overflow handling
  // http://stackoverflow.com/questions/7067357/catching-stack-overflow
  char stack;
  if (first_stack == NULL) {
    first_stack = &stack;
  } else if ((first_stack > &stack && first_stack - &stack > MAX_STACK)  ||
        (&stack > first_stack && &stack - first_stack > MAX_STACK)) {
    return 1;
  }
  
  return 0;
}
