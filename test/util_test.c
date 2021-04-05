#include "list.h"
#include "log.h"
#include <stdio.h>

int main() {
  list *li = list_init();
  char *things = "asdf";
  char *things1 = "asdf1";
  char *things2 = "asdf2";
  list_add(li, things);
  list_add(li, things1);
  list_add(li, things2);

  node *iter = li->head;
  while (iter) {
    log_info("%s", iter->data);
    iter = iter->next;
  }
  log_info("%d", list_size(li));
  // log_info("%s", list_at(li, 0));

  list_remove(li, 0);
  list_remove(li, 0);

  iter = li->head;
  while (iter) {
    log_info("%s", iter->data);
    iter = iter->next;
  }

  list_deinit(li);

  return 0;
}
