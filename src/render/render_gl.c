#include "render_gl.h"
#include "log.h"

static int counter;

static int init() {
  counter = 0;
  log_debug("gl render init");
  return 0;
}

static int draw() {
  log_debug("gl render draw for %d time", ++counter);
  return 0;
}

render_interface render_gl = {
  &init,
  &draw
};
