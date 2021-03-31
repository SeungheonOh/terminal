#if defined(__APPLE__)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <stdio.h>
#include <string.h>

#include "log.h"
#include "render/render.h"

#include <GLFW/glfw3.h>

void test_input_cb(GLFWwindow *win, int key, int scancode, int action,
                   int mods) {
  if (key == GLFW_KEY_ESCAPE)
    glfwSetWindowShouldClose(win, GL_TRUE);
}

int main() {
  set_loglevel(LOG_ALL);

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(500, 500, "Terminal", NULL, NULL);
  if (window == NULL) {
    log_error("failed to create glfw window");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwSetKeyCallback(window, test_input_cb);

  glewExperimental = 1;
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    // Problem: glewInit failed, something is seriously wrong.
    log_error("GLEW: %s", glewGetErrorString(err));
    /*
    exit(EXIT_FAILURE);
    */
  }
  fprintf(stderr, "Using GLEW %s\n", glewGetString(GLEW_VERSION));

  render_gl.init();

  while (!glfwWindowShouldClose(window)) {
    render_gl.draw();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
