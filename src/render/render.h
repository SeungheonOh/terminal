#include "GL/glew.h"

#include "GL/gl.h"

typedef struct {
  int (*init)();
  int (*draw)();
} render_interface;

extern render_interface render_gl;
