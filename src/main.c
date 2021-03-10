#if defined(__APPLE__)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

// Triangle with vertex-buffer
#include <stdio.h>
#include <string.h>

#include "mat4.h"
#include "shader.h"
#include "vertex-buffer.h"

#include <GLFW/glfw3.h>

// ------------------------------------------------------- global variables ---
GLuint shader;
vertex_buffer_t *cube;
mat4 model, view, projection;

// ------------------------------------------------------------------- init ---
void init(void) {
  typedef struct {
    float x, y, z;
  } xyz;
  typedef struct {
    float r, g, b, a;
  } rgba;
  typedef struct {
    xyz position;
    rgba color;
  } vertex;

  rgba c[] = {{1, 1, 1, 1}, {1, 1, 0, 1}, {1, 0, 1, 1}, {0, 1, 1, 1},
              {1, 0, 0, 1}, {0, 0, 1, 1}, {0, 1, 0, 1}, {0, 0, 0, 1}};

  vertex vertices[] = {
      {{-0.5f, -0.5f, 0.0f}, c[1]}, // left
      {{0.5f, -0.5f, 0.0f}, c[2]},  // right
      {{0.0f, 0.5f, 0.0f}, c[3]}    // top
  };

  GLuint indices[3] = {0, 1, 2};

  cube = vertex_buffer_new("vertex:3f,color:4f");
  vertex_buffer_push_back(cube, vertices, 3, indices, 3);
  shader =
      shader_load("/Users/seungheonoh/Documents/Terminal/src/shaders/1.vert",
                  "/Users/seungheonoh/Documents/Terminal/src/shaders/1.frag");

  glPolygonOffset(1, 1);
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LINE_SMOOTH);
}

// ---------------------------------------------------------------- display ---
void display(GLFWwindow *window) {
  static float theta = 0, phi = 0;
  static GLuint Color = 0;
  double seconds_elapsed = glfwGetTime();

  if (!Color) {
    Color = glGetUniformLocation(shader, "Color");
  }

  theta = .5f * seconds_elapsed / 0.016f;
  phi = .5f * seconds_elapsed / 0.016f;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POLYGON_OFFSET_FILL);

  glUseProgram(shader);

  glUniform4f(Color, 1, 1, 1, 1);
  vertex_buffer_render(cube, GL_TRIANGLES);

  glfwSwapBuffers(window);
}

// ---------------------------------------------------------------- reshape ---
void reshape(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// --------------------------------------------------------------- keyboard ---
void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

// --------------------------------------------------------- error-callback ---
void error_callback(int error, const char *description) {
  fputs(description, stderr);
}

// ------------------------------------------------------------------- main ---
int main(int argc, char **argv) {
  GLFWwindow *window;
  char *screenshot_path = NULL;

  if (argc > 1) {
    if (argc == 3 && 0 == strcmp("--screenshot", argv[1]))
      screenshot_path = argv[2];
    else {
      fprintf(stderr, "Unknown or incomplete parameters given\n");
      exit(EXIT_FAILURE);
    }
  }

  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window = glfwCreateWindow(400, 400, argv[0], NULL, NULL);

  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glfwSetFramebufferSizeCallback(window, reshape);
  glfwSetWindowRefreshCallback(window, display);
  glfwSetKeyCallback(window, keyboard);

  init();

  glfwShowWindow(window);
  {
    int pixWidth, pixHeight;
    glfwGetFramebufferSize(window, &pixWidth, &pixHeight);
    reshape(window, pixWidth, pixHeight);
  }

  glfwSetTime(1.0);

  while (!glfwWindowShouldClose(window)) {
    display(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}

/*
// Text printing

#include <stdio.h>
#include <string.h>

#include "font-manager.h"
#include "log.h"
#include "markup.h"
#include "shader.h"
#include "text-buffer.h"
#include "vertex-buffer.h"

#include <GLFW/glfw3.h>
#include <fontconfig/fontconfig.h>

char *match_font_description(const char *desc) {
#if defined(_WIN32) || defined(_WIN64)
  die(-1, "Windows doesn't have fontconfig, too bad");
#endif
  char *filename = 0;
  FcInit();
  FcPattern *pattern = FcNameParse((const FcChar8 *)desc);
  FcConfigSubstitute(0, pattern, FcMatchPattern);
  FcDefaultSubstitute(pattern);
  FcResult result;
  FcPattern *match = FcFontMatch(0, pattern, &result);
  FcPatternDestroy(pattern);

  if (!match) {
    die(-1, "font description not found: %s", desc);
  } else {
    FcValue value;
    FcResult result = FcPatternGet(match, FC_FILE, 0, &value);
    if (result) {
      die(-1, "font description not found: %s", desc);
    } else {
      filename = strdup((char *)(value.u.s));
    }
  }
  FcPatternDestroy(match);
  return filename;
}

typedef struct {
  float x, y, z; // pos
  float s, t;    // texture
  float r, g, b; // color
} vertex;

// ------------------------------------------------------- global variables ---
GLuint shader;
GLuint textshader;
vertex_buffer_t *cube;
text_buffer_t *textbuffer;
font_manager_t *fontmanager;

void init(void) {

  fontmanager = font_manager_new(512, 512, LCD_FILTERING_ON);
  textbuffer = text_buffer_new();

  char *f_normal = match_font_description("IBM Plex Mono");
  vec4 white = {{1.0, 1.0, 1.0, 1.0}};
  vec4 none = {{1.0, 1.0, 1.0, 1.0}};

  markup_t normal = {
      .family = f_normal,
      .size = 24.0,
      .bold = 0,
      .italic = 0,
      .spacing = 0.0,
      .gamma = 2.,
      .foreground_color = white,
      .background_color = none,
      .underline = 0,
      .underline_color = white,
      .overline = 0,
      .overline_color = white,
      .strikethrough = 0,
      .strikethrough_color = white,
      .font = 0,
  };

  normal.font = font_manager_get_from_markup(fontmanager, &normal);

  vec2 pen = {{20, 200}};
  text_buffer_printf(textbuffer, &pen, &normal, "Thonk, Stonk", NULL);

  glGenTextures(1, &fontmanager->atlas->id);
  glBindTexture(GL_TEXTURE_2D, fontmanager->atlas->id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fontmanager->atlas->width,
               fontmanager->atlas->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
               fontmanager->atlas->data);

  typedef struct {
    float x, y, z;
  } xyz;
  typedef struct {
    float r, g, b, a;
  } rgba;
  typedef struct {
    xyz position;
    rgba color;
  } vertex;

  rgba c[] = {{1, 1, 1, 1}, {1, 1, 0, 1}, {1, 0, 1, 1}, {0, 1, 1, 1},
              {1, 0, 0, 1}, {0, 0, 1, 1}, {0, 1, 0, 1}, {0, 0, 0, 1}};

  vertex vertices[] = {
      {{-0.5f, -0.5f, 0.0f}, c[1]}, // left
      {{0.5f, -0.5f, 0.0f}, c[2]},  // right
      {{0.0f, 0.5f, 0.0f}, c[3]}    // top
  };

  GLuint indices[3] = {0, 1, 2};

  cube = vertex_buffer_new("vertex:3f,color:4f");
  vertex_buffer_push_back(cube, vertices, 3, indices, 3);
  shader =
      shader_load("/Users/SeungheonOh/Documents/Terminal/src/shaders/1.vert",
                  "/Users/SeungheonOh/Documents/Terminal/src/shaders/1.frag");
  textshader =
      shader_load("/Users/SeungheonOh/Documents/Terminal/src/shaders/2.vert",
                  "/Users/SeungheonOh/Documents/Terminal/src/shaders/2.frag");
  glPolygonOffset(1, 1);
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LINE_SMOOTH);
}

// ---------------------------------------------------------------- display ---
void display(GLFWwindow *window) {
  static float theta = 0, phi = 0;
  static GLuint Color = 0;
  double seconds_elapsed = glfwGetTime();

  if (!Color) {
    Color = glGetUniformLocation(shader, "Color");
  }

  theta = .5f * seconds_elapsed / 0.016f;
  phi = .5f * seconds_elapsed / 0.016f;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POLYGON_OFFSET_FILL);

  glUseProgram(shader);

  glUniform4f(Color, 1, 1, 1, 1);
  vertex_buffer_render(cube, GL_TRIANGLES);

  // text
  glClearColor(0.40, 0.40, 0.45, 1.00);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glColor4f(1.00, 1.00, 1.00, 1.00);
  glUseProgram(shader);
  {
    glUniform1i(glGetUniformLocation(textshader, "tex"), 0);
    glUniform3f(glGetUniformLocation(textshader, "pixel"),
                1.0f / fontmanager->atlas->width,
                1.0f / fontmanager->atlas->height,
                (float)fontmanager->atlas->depth);

    glEnable(GL_BLEND);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontmanager->atlas->id);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendColor(1, 1, 1, 1);

    vertex_buffer_render(textbuffer->buffer, GL_TRIANGLES);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBlendColor(0, 0, 0, 0);
    glUseProgram(0);
  }

  glfwSwapBuffers(window);
}

// ---------------------------------------------------------------- reshape ---
void reshape(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// --------------------------------------------------------------- keyboard ---
void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

// --------------------------------------------------------- error-callback ---
void error_callback(int error, const char *description) {
  fputs(description, stderr);
}

// ------------------------------------------------------------------- main ---
int main(int argc, char **argv) {
  log_info("starting");
  GLFWwindow *window;
  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window = glfwCreateWindow(400, 400, argv[0], NULL, NULL);

  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glfwSetFramebufferSizeCallback(window, reshape);
  glfwSetWindowRefreshCallback(window, display);
  glfwSetKeyCallback(window, keyboard);

  init();

  glfwShowWindow(window);
  {
    int pixWidth, pixHeight;
    glfwGetFramebufferSize(window, &pixWidth, &pixHeight);
    reshape(window, pixWidth, pixHeight);
  }

  glfwSetTime(1.0);

  while (!glfwWindowShouldClose(window)) {
    display(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}
*/
