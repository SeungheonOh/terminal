#if defined(__APPLE__)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <stdio.h>
#include <string.h>

#include "render/render_gl.h"
#include "log.h"

int main() {
  render_gl.init();
  render_gl.draw();
  render_gl.draw();
  render_gl.draw();
  set_loglevel(LOG_ALL);
  log_info("hello world");
  sayhello();
}
