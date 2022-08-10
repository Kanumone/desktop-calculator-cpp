#include <stdio.h>

#include "s21_calculator.h"
#include "s21_parser.h"

void print_stack(s21_deque_t *stack) {
  int n = stack->size;
  list_t *current_node = stack->front;
  if (n == -1) printf("Stack is empty");
  while (n >= 0) {
    if (current_node->lex.priority < 0) {
      printf("|%f|\n", current_node->lex.data.operand);
    } else {
      printf("|%c|\n", current_node->lex.data.opr + 42);
    }
    current_node = current_node->prev;
    n--;
  }
}

int main(void) {
  // Test calc

  char *test = (char *)malloc(100);
  snprintf(test, 99, "x^2");
  s21_deque_t *rpn = s21_parse_exp(test);
  for (double i = -3; i - 3.0 <= 1e-7; i += 0.05) {
    double result = s21_calculate_rpn(rpn, i);
    printf("x = %f, y = %f\n", i, result);
  }
  s21_delete_deque(rpn);
  free(test);

  // Test stack

  // lex_t lex = {.priority = S21_NUMBER, .data.operand = 10.0};
  // s21_deque_t *deque = s21_create_deque();
  // s21_push_front(deque, lex);
  // s21_push_front(deque, lex);
  // s21_push_front(deque, lex);
  // s21_pop_front(deque);
  // s21_pop_front(deque);
  // s21_pop_front(deque);
  // s21_delete_deque(deque);

  return 0;
}
