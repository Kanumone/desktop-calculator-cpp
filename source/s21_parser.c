#include "s21_parser.h"

char *get_number(char *exp_ptr, lex_t *number) {
  if (*exp_ptr == 'x') {
    exp_ptr++;
    number->data.opr = 15;
  } else {
    char buff[70];
    size_t i = 0;
    while ((*exp_ptr >= '0' && *exp_ptr <= '9') || *exp_ptr == '.' || *exp_ptr == 'E') {
      if (*exp_ptr == 'E') {
        buff[i++] = *exp_ptr;
        exp_ptr++;
      }
      buff[i++] = *exp_ptr;
      exp_ptr++;
    }
    buff[i] = '\0';
    number->data.operand = atof(buff);
  }

  number->priority = S21_NUMBER;

  return exp_ptr;
}

char *get_opr(char *exp_ptr, lex_t *op) {
  if (*exp_ptr == '+' || *exp_ptr == '-') {
    op->priority = S21_LOW;
    op->data.opr = (*exp_ptr) - 42;
    exp_ptr++;
  } else if (*exp_ptr == '/' || *exp_ptr == '*') {
    op->priority = S21_MIDDLE;
    op->data.opr = (*exp_ptr) - 42;
    exp_ptr++;
  } else if (*exp_ptr == '^') {
    op->priority = S21_HIGH;
    op->data.opr = 2;
    exp_ptr++;
  } else if (strncmp(exp_ptr, "mod", 3) == 0) {
    op->priority = S21_MIDDLE;
    op->data.opr = 4;
    exp_ptr += 3;
  } else if (strncmp(exp_ptr, "sin", 3) == 0) {
    op->priority = S21_FUNC;
    op->data.opr = 6;
    exp_ptr += 3;
  } else if (strncmp(exp_ptr, "cos", 3) == 0) {
    op->priority = S21_FUNC;
    op->data.opr = 7;
    exp_ptr += 3;
  } else if (strncmp(exp_ptr, "tan", 3) == 0) {
    op->priority = S21_FUNC;
    op->data.opr = 8;
    exp_ptr += 3;
  } else if (strncmp(exp_ptr, "asin", 4) == 0) {
    op->priority = S21_FUNC;
    op->data.opr = 9;
    exp_ptr += 4;
  } else if (strncmp(exp_ptr, "acos", 4) == 0) {
    op->priority = S21_FUNC;
    op->data.opr = 10;
    exp_ptr += 4;
  } else if (strncmp(exp_ptr, "atan", 4) == 0) {
    op->priority = S21_FUNC;
    op->data.opr = 11;
    exp_ptr += 4;
  } else if (strncmp(exp_ptr, "log", 3) == 0) {
    op->priority = S21_FUNC;
    op->data.opr = 12;
    exp_ptr += 3;
  } else if (strncmp(exp_ptr, "ln", 2) == 0) {
    op->priority = S21_FUNC;
    op->data.opr = 13;
    exp_ptr += 2;
  } else if (strncmp(exp_ptr, "sqrt", 4) == 0) {
    op->priority = S21_FUNC;
    op->data.opr = 14;
    exp_ptr += 4;
  }
  return exp_ptr;
}

/**
 * @brief Функция, производящая парсинг входной строки в обратную польскую нотацию
 *
 * @param exp Входная строка
 * @return Очередь с полученной обратной польской нотацией
 */
s21_deque_t *s21_parse_exp(char *exp) {
  char *exp_ptr = exp;
  lex_t prev_lex = {.priority = S21_ERROR};
  s21_deque_t *sort_stack = s21_create_deque();
  s21_deque_t *result = s21_create_deque();

  while (*exp_ptr) {
    if (*exp_ptr == ' ') {
      exp_ptr++;
    } else if ((*exp_ptr >= '0' && *exp_ptr <= '9') || *exp_ptr == 'x') {
      lex_t number = {0};
      exp_ptr = get_number(exp_ptr, &number);
      prev_lex = number;
      s21_push_front(result, number);
    } else {
      if (*exp_ptr == '(') {
        lex_t right_bracket = {0};
        right_bracket.priority = S21_BRACKET;
        prev_lex = right_bracket;
        s21_push_front(sort_stack, right_bracket);
        exp_ptr++;
      } else if (*exp_ptr == ')') {
        while (sort_stack->front->lex.priority != S21_BRACKET) {
          lex_t current_lex = s21_pop_front(sort_stack);
          s21_push_front(result, current_lex);
        }
        s21_pop_front(sort_stack);
        exp_ptr++;
      } else {
        lex_t op = {0};
        exp_ptr = get_opr(exp_ptr, &op);
        if ((op.data.opr == 3 || op.data.opr == 1) &&
            ((prev_lex.priority > S21_NUMBER && prev_lex.priority <= 5) ||
             prev_lex.priority == S21_ERROR)) {
          op.priority = S21_UNARY;
          s21_push_front(sort_stack, op);
        } else {
          while (sort_stack->size >= 0 && sort_stack->front->lex.priority >= op.priority) {
            lex_t current_lex = s21_pop_front(sort_stack);
            s21_push_front(result, current_lex);
          }
          s21_push_front(sort_stack, op);
        }
        prev_lex = op;
      }
    }
  }

  while (sort_stack->size >= 0) {
    lex_t current_lex = s21_pop_front(sort_stack);
    s21_push_front(result, current_lex);
  }

  s21_delete_deque(sort_stack);

  return result;
}
