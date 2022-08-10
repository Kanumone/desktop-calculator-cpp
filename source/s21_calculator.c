#include "s21_calculator.h"

double s21_sum(double a, double b) { return a + b; }

double s21_sub(double a, double b) { return a - b; }

double s21_mul(double a, double b) { return a * b; }

double s21_div(double a, double b) { return a / b; }

double (*binary_function[])(double, double) = {s21_mul, s21_sum, pow, s21_sub, fmod, s21_div};
double (*unary_function[])(double) = {NULL, NULL, NULL, NULL, NULL,  NULL, sin, cos,
                                      tan,  asin, acos, atan, log10, log,  sqrt};

double s21_calculate_rpn(s21_deque_t *rpn, double x) {
  s21_deque_t *calc_stack = s21_create_deque();
  list_t *back_address = rpn->back;
  int rpn_size = rpn->size;
  while (rpn_size >= 0) {
    if (rpn->back->lex.priority == S21_NUMBER) {
      lex_t num_x = rpn->back->lex;
      if (num_x.data.opr == 15) {
        num_x.data.operand = x;
      }
      s21_push_front(calc_stack, num_x);
    } else {
      if (rpn->back->lex.priority == S21_UNARY && rpn->back->lex.data.opr == 3) {
        calc_stack->front->lex.data.operand *= -1;
      } else if (rpn->back->lex.data.opr >= 0 && rpn->back->lex.data.opr <= 5) {
        double opd_2 = s21_pop_front(calc_stack).data.operand;
        double opd_1 = s21_pop_front(calc_stack).data.operand;
        lex_t num = {.priority = S21_NUMBER};
        num.data.operand = binary_function[rpn->back->lex.data.opr](opd_1, opd_2);
        s21_push_front(calc_stack, num);
      } else if (rpn->back->lex.data.opr > 5) {
        double opd = s21_pop_front(calc_stack).data.operand;
        lex_t num = {.priority = S21_NUMBER};
        num.data.operand = unary_function[rpn->back->lex.data.opr](opd);
        s21_push_front(calc_stack, num);
      }
    }

    if (rpn_size > 0) {
      rpn->back = rpn->back->next;
    }
    rpn_size--;
  }
  double result = s21_pop_front(calc_stack).data.operand;
  s21_delete_deque(calc_stack);
  rpn->back = back_address;
  return result;
}
