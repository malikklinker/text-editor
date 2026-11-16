#include "editor_lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Functions about ll_text...
// append a line: add to the end of the linked list
ll_text *append_text(ll_text *list, char *text) {
  char *new_text = strdup(text);
  ll_text *new_ll = calloc(1, sizeof(ll_text));
  new_ll->text = new_text;
  new_ll->next = NULL;
  if (!list) {
    return new_ll;
  }
  ll_text *cur = list;
  while (cur->next) {
    cur = cur->next;
  }
  cur->next = new_ll;
  return list;
}

void save_to_file(ll_text *list, const char *filename) {
  FILE *outfile = fopen(filename, "w");
  if (!outfile) {
    perror(filename);
    return;
  }
  for (ll_text *here = list; here; here = here->next) {
    fprintf(outfile, "%s\n", here->text);
  }
  if (fclose(outfile) != 0)
    perror(filename);
}

int ll_text_length(ll_text *list) {
  int count = 0;
  while (list) {
    list = list->next;
    count++;
  }
  return count;
}

// insert a specific line: add to specific place in the linked list; this is the
// new entry at the specified line
ll_text *insert_text(ll_text *list, char *text, int position) {
  if (position < 0 || position > ll_text_length(list))
    return list;
  ll_text *new_ll = malloc(sizeof(ll_text));
  new_ll->text = strdup(text);
  new_ll->next = list;
  ll_text *cur = list;

  if (position == 0) {
    return new_ll;
  }
  for (int i = 0; i < position - 1; i++) {
    cur = cur->next;
  }
  new_ll->next = cur->next;
  cur->next = new_ll;
  return list;
}

// delete a specific line: delete the ith entry in the linked list and return
// the new front of the linked list.
ll_text *delete_text(ll_text *list, int position) {
  if (position < 0 || position >= ll_text_length(list))
    return list;
  if (!list) {
    return NULL;
  }
  if (!(list->next)) {
    free(list->text);
    free(list);
    return NULL;
  }
  if (position == 0) {
    ll_text *next = list->next;
    free(list->text);
    free(list);
    return next;
  }
  ll_text *cur = list;
  for (int i = 0; i < position - 1; i++) {
    cur = cur->next;
  }
  ll_text *ll_rm = cur->next;
  cur->next = ll_rm->next;
  free(ll_rm->text);
  free(ll_rm);
  return list;
}

// replace a line: go to the specific entry, free the text that's there, replace
// it with a copy of the specified text.
ll_text *replace_text(ll_text *list, char *text, int position) {
  if (position < 0 || position >= ll_text_length(list))
    return list;
  ll_text *cur = list;
  for (int i = 0; i < position; i++) {
    cur = cur->next;
  }
  char *replacement = strdup(text);
  free(cur->text);
  cur->text = replacement;
  return list;
}

// duplicate a list. Returns a copy of the list, including newly copied versions
// of all of the strings.
ll_text *duplicate_ll_text(ll_text *list) {
  if (!list) {
    return NULL;
  }
  ll_text *new_head = malloc(sizeof(ll_text));
  new_head->text = strdup(list->text);
  new_head->next = NULL;
  ll_text *cur = list->next;
  ll_text *cur_new = new_head;
  while (cur) {
    cur_new->next = malloc(sizeof(ll_text));
    cur_new = cur_new->next;
    cur_new->text = strdup(cur->text);
    cur_new->next = NULL;
    cur = cur->next;
  }

  return new_head;
}

// functions about the state stack...
ll_text_stack *load_from_file(const char *filename) {
  ll_text *lines = NULL;
  ll_text_stack *stack = calloc(1, sizeof(ll_text_stack));
  char buf[1024];
  FILE *infile = fopen(filename, "r");
  if (!infile) {
    perror(filename);
    free(stack);
    return NULL;
  }
  while (fgets(buf, 1024, infile)) {
    chomp(buf);
    lines = append_text(lines, buf);
  }
  stack->lines = lines;
  fclose(infile);
  return stack;
}

ll_text_stack *new_stack_empty_text(void) {
  ll_text_stack *stack = calloc(1, sizeof(ll_text_stack));
  stack->lines = NULL;
  return stack;
}

// Push a new state onto the stack that's an exact copy of the state currently
// at the top.

ll_text_stack *push_duplicate(ll_text_stack *stack) {
  ll_text_stack *ll_new = malloc(sizeof(ll_text_stack));
  ll_new->lines = duplicate_ll_text(stack ? stack->lines : NULL);
  ll_new->next = stack;
  return ll_new;
}

// Push a new entry onto the stack that has an empty ll_text at the top.
ll_text_stack *push_empty(ll_text_stack *stack) {
  ll_text_stack *ll_new = malloc(sizeof(ll_text_stack));
  ll_new->next = stack;
  ll_new->lines = NULL;
  return ll_new;
}

// undo, removing the most recent state
// this has to free all the memory associated with that state
ll_text_stack *pop_stack(ll_text_stack *stack) {
  if (!stack)
    return NULL;
  ll_text_stack *next = stack->next;
  ll_text *line = stack->lines;
  while (line) {
    ll_text *next_line = line->next;
    free(line->text);
    free(line);
    line = next_line;
  }
  free(stack);
  return next;
}

// Find the first newline in the input and replace it with \0
void chomp(char *s) {
  for (int i = 0; s[i]; i++) {
    if (s[i] == '\n' || s[i] == '\r') {
      // Check for \r in case we're on Windows or a very old Mac??
      s[i] = '\0';
      break;
    }
  }
}

ll_text_stack *push_lines(ll_text_stack *stack, ll_text *lines) {
  ll_text_stack *ll_new = malloc(sizeof(ll_text_stack));
  ll_new->lines = lines;
  ll_new->next = stack;
  return ll_new;
}

void clear_stack(ll_text_stack *stack) {
  while (stack) {
    stack = pop_stack(stack);
  }
}
