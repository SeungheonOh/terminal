typedef struct node {
  void* data;
  struct node* next;
} node;

typedef struct {
  unsigned int size;
  node* head;
  node* tail;
} list;

list* list_init();
void list_add(list*, void*);
void list_remove(list*, unsigned int);
void* list_at(list*, unsigned int);
int list_size(list*);
node* list_iter(list*);
void list_deinit(list*);
