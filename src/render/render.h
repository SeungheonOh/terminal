#include "opengl.h"

// TODO Need better interface
typedef struct {
  int (*init)();
  int (*draw)();
} render_interface;

extern render_interface render_gl;
