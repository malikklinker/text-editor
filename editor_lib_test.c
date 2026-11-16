#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "editor_lib.h"

static int failures;

static void expect_string(const char *message, const char *expected,
                          const char *actual) {
  if (!actual || strcmp(expected, actual)) {
    fprintf(stderr, "%s: expected \"%s\", got \"%s\"\n", message, expected,
            actual ? actual : "(null)");
    ++failures;
  }
}

static void expect_int(const char *message, int expected, int actual) {
  if (expected != actual) {
    fprintf(stderr, "%s: expected %d, got %d\n", message, expected, actual);
    ++failures;
  }
}

int main(void) {
  ll_text *list = NULL;
  list = append_text(list, "hello");

  expect_string("empty list and then append hello", "hello", list->text);
  free(list->text);
  free(list);

  list = NULL;
  list = append_text(list, "hello");
  list = append_text(list, "second string");
  expect_string("empty list and then append two strings", "second string",
                list->next->text);
  free(list->next->text);
  free(list->next);
  free(list->text);
  free(list);

  list = NULL;
  list = append_text(list, "hello");
  list = insert_text(list, "new first string", 0);
  expect_string("empty list and then append and then insert",
                "new first string", list->text);

  list = insert_text(list, "one", 1);
  expect_string("empty list and then append and then insert", "one",
                list->next->text);
  expect_string("empty list and then append and then insert", "hello",
                list->next->next->text);

  free(list->next->next->text);
  free(list->next->next);
  free(list->next->text);
  free(list->next);
  free(list->text);
  free(list);

  list = NULL;
  list = append_text(list, "hello");
  list = insert_text(list, "new first string", 0);
  list = insert_text(list, "one", 1);

  list = delete_text(list, 1);

  expect_string("append and then insert and then delete", "new first string",
                list->text);
  expect_string("append and then insert and then delete", "hello",
                list->next->text);

  list = delete_text(list, 0);
  expect_string("append and then insert and then delete", "hello", list->text);
  free(list->text);
  free(list);

  list = NULL;
  list = append_text(list, "hello");
  list = insert_text(list, "new first string", 0);
  list = insert_text(list, "one", 1);

  list = replace_text(list, "two", 1);
  expect_string("append, two inserts, then replace", "two", list->next->text);

  list = replace_text(list, "good evening", 0);
  expect_string("append, two inserts, then replace", "good evening",
                list->text);
  free(list->next->next->text);
  free(list->next->next);
  free(list->next->text);
  free(list->next);
  free(list->text);
  free(list);

  list = NULL;
  list = append_text(list, "one");
  list = append_text(list, "two");
  list = append_text(list, "three");

  ll_text *newlist = duplicate_ll_text(list);
  expect_string("copied list", "one", newlist->text);
  expect_string("copied list", "three", newlist->next->next->text);

  expect_int("snapshot owns its nodes", 1, list != newlist);

  expect_int("snapshot owns its text", 1, list->text != newlist->text);

  free(list->next->next->text);
  free(list->next->next);
  free(list->next->text);
  free(list->next);
  free(list->text);
  free(list);

  free(newlist->next->next->text);
  free(newlist->next->next);
  free(newlist->next->text);
  free(newlist->next);
  free(newlist->text);
  free(newlist);

  list = NULL;

  expect_int("length of empty list should be 0", 0, ll_text_length(list));

  list = append_text(list, "one");
  list = append_text(list, "two");
  list = append_text(list, "three");

  expect_int("length of list should now be 3", 3, ll_text_length(list));

  free(list->next->next->text);
  free(list->next->next);
  free(list->next->text);
  free(list->next);
  free(list->text);
  free(list);

  list = NULL;
  list = append_text(list, "one");

  ll_text_stack *stack = calloc(1, sizeof(ll_text_stack));
  stack->lines = list;

  stack = push_duplicate(stack);
  expect_string("made a new entry in the stack", "one", stack->lines->text);
  expect_string("old entry is still there", "one", stack->next->lines->text);

  free(stack->next->lines->text);
  free(stack->next->lines);
  free(stack->next);
  free(stack->lines->text);
  free(stack->lines);
  free(stack);

  list = NULL;
  list = append_text(list, "one");
  stack = calloc(1, sizeof(ll_text_stack));
  stack->lines = list;

  stack = push_empty(stack);
  expect_int("empty snapshot", 1, stack->lines == NULL);
  expect_string("old entry is still there", "one", stack->next->lines->text);

  free(stack->next->lines->text);
  free(stack->next->lines);
  free(stack->next);
  free(stack->lines);
  free(stack);

  list = NULL;
  list = append_text(list, "one");
  stack = calloc(1, sizeof(ll_text_stack));
  stack->lines = list;

  stack = push_empty(stack);
  stack = pop_stack(stack);
  expect_string("old entry is restored after popping", "one",
                stack->lines->text);

  free(stack->lines->text);
  free(stack->lines);
  free(stack);

  return failures ? 1 : 0;
}
