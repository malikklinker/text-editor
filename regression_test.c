#include "editor_lib.h"
#include <assert.h>
#include <string.h>

int main(void) {
  ll_text *lines = append_text(NULL, "first");
  assert(delete_text(lines, 1) == lines);
  assert(delete_text(lines, -1) == lines);
  assert(insert_text(lines, "bad", 2) == lines);
  assert(replace_text(lines, "bad", -1) == lines);
  lines = replace_text(lines, "replacement", 0);
  assert(!strcmp(lines->text, "replacement"));
  ll_text_stack *stack = push_lines(NULL, lines);
  stack = push_duplicate(stack);
  stack->lines = append_text(stack->lines, "second");
  assert(ll_text_length(stack->lines) == 2);
  assert(ll_text_length(stack->next->lines) == 1);
  stack = pop_stack(stack);
  assert(ll_text_length(stack->lines) == 1);
  clear_stack(stack);
  assert(pop_stack(NULL) == NULL);
  return 0;
}
