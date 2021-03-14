#include "render_gl.h"
#include "log.h"

static int counter;

int init() {
  counter = 0;
  log_debug("gl render init");
  return 0;
}

int draw() {
  log_debug("gl render draw for %d time", ++counter);
  return 0;
}

render_interface render_gl = {
  &init,
  &draw
};
