#if defined(__APPLE__)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <stdio.h>
#include <string.h>

#include "render/render_test.h"
#include "log.h"

int main() {
  log_info("hello world");
  sayhello();
}
