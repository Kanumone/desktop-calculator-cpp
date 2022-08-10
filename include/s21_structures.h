#ifndef SRC_INCLUDE_S21_STRUCTURES_H_
#define SRC_INCLUDE_S21_STRUCTURES_H_

#include <stdlib.h>

enum priority {
  S21_ERROR = -3,
  S21_NUMBER,
  S21_BRACKET,
  S21_LOW,
  S21_MIDDLE,
  S21_HIGH,
  S21_UNARY,
  S21_FUNC
};

typedef struct lex {
  short priority;
  union {
    double operand;
    int opr;
  } data;
} lex_t;

typedef struct node {
  struct node *next;
  struct node *prev;
  lex_t lex;
} list_t;

list_t *add_after(list_t *current_node, lex_t lex);
list_t *add_before(list_t *current_node, lex_t lex);
void delete_node(list_t *node);

typedef struct s21_deque {
  list_t *front;
  list_t *back;
  int size;
} s21_deque_t;

s21_deque_t *s21_create_deque();
void s21_delete_deque(s21_deque_t *deque);
void s21_clear_deque(s21_deque_t *stack);
lex_t s21_pop_front(s21_deque_t *stack);
void s21_push_front(s21_deque_t *stack, lex_t lex);

#endif  // SRC_INCLUDE_S21_STRUCTURES_H_
