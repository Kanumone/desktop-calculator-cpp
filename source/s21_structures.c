#include "s21_structures.h"

list_t *add_after(list_t *current_node, lex_t lex) {
  list_t *new_node = (list_t *)calloc(1, sizeof(list_t));
  new_node->lex = lex;
  if (current_node->next == NULL) {
    current_node->next = new_node;
    new_node->prev = current_node;
    new_node->next = NULL;
  } else {
    current_node->next->prev = new_node;
    new_node->next = current_node->next;
    current_node->next = new_node;
    new_node->prev = current_node;
  }

  return new_node;
}

void delete_node(list_t *node) {
  if (node->prev != NULL) {
    node->prev->next = node->next;
  }
  if (node->next != NULL) {
    node->next->prev = node->prev;
  }
  free(node);
}

/**
 * @brief Функций для создания двунаправленной очереди
 *
 * @return Указатель на созданную двунаправленную очередь
 */
s21_deque_t *s21_create_deque() {
  s21_deque_t *deque = (s21_deque_t *)malloc(sizeof(s21_deque_t));
  deque->front = (list_t *)calloc(1, sizeof(list_t));
  deque->back = deque->front;
  deque->size = -1;
  return deque;
}

/**
 * @brief Функция для очистки двунаправленной очереди
 *
 * @param deque Двунаправленная очередь, которую необходимо очистить
 */
void s21_clear_deque(s21_deque_t *deque) {
  while (deque->size > 0) {
    deque->front = deque->front->prev;
    delete_node(deque->front->next);
    deque->size -= 1;
  }
  deque->size -= 1;
}

/**
 * @brief Функция для удаления двунаправленной очереди
 *
 * @param deque Двунаправленная очередь, которую необходимо удалить
 */
void s21_delete_deque(s21_deque_t *deque) {
  s21_clear_deque(deque);
  free(deque->front);
  free(deque);
}

/**
 * @brief Функция для добавления элемента в начало двунаправленной очереди
 *
 * @param deque Указатель на двунаправленную очередь, в которую необходимо добавить элемент
 * @param lex Лексема, которую необходимо добавить в двунаправленную очередь
 */
void s21_push_front(s21_deque_t *deque, lex_t lex) {
  if (deque->size >= 0) {
    deque->front = add_after(deque->front, lex);
  } else {
    deque->front->lex = lex;
  }
  deque->size += 1;
}

/**
 * @brief Функция для удаления элемента из начала двунаправленной очереди
 *
 * @return Элемент, удаляемый из начала очереди
 */

lex_t s21_pop_front(s21_deque_t *deque) {
  lex_t result = {0};
  if (deque->size >= 0) {
    result = deque->front->lex;
    if (deque->size != 0) {
      deque->front = deque->front->prev;
      delete_node(deque->front->next);
    }
    deque->size -= 1;
  } else {
    result.priority = S21_ERROR;
  }
  return result;
}
