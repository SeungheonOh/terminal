#include "list.h"
#include <stdlib.h>

list *list_init() {
  list *n = malloc(sizeof(list));
  if (n == NULL)
    return NULL;
  n->size = 0;
  n->head = NULL;
  n->tail = NULL;
  return n;
}
void list_add(list *l, void *d) {
  if (l == NULL)
    return;

  node *n = malloc(sizeof(node));
  if (n == NULL)
    return;
  n->data = d;
  n->next = NULL;

  if (l->head == NULL) {
    l->head = l->tail = n;
    return;
  }
  l->tail = l->tail->next = n;
  l->size++;
  return;
};
void list_remove(list *l, unsigned int indx) {
  if (l == NULL || list_size(l) <= indx)
    return;

  node *iter = l->head;
  node *tmp;
  if (indx == 0) {
    tmp = l->head;
    l->head = l->head->next;
    free(tmp);
  } else {
    while (indx-- > 1 && iter != NULL)
      iter = iter->next;
    tmp = iter->next;
    iter->next = iter->next->next;
    free(tmp);
  }
  l->size--;
  return;
}
void *list_at(list *l, unsigned int indx) {
  if (l == NULL)
    return NULL;
  node *iter = l->head;
  while (indx-- > 0 && iter != NULL)
    iter = iter->next;
  return iter->data;
}
int list_size(list *l) { return (l == NULL) ? -1 : l->size; };
node *list_iter(list *l) { return (l == NULL) ? NULL : l->head; }
void list_deinit(list *l) {
  if (l == NULL)
    return;
  node *iter = l->head;
  node *tmp;
  while (iter != NULL) {
    tmp = iter;
    iter = iter->next;
    free(tmp);
  }
  free(l);
}
