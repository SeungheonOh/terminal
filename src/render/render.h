typedef struct {
  int (*init)();
  int (*draw)();
} render_interface;
