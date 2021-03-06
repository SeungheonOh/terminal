/* Freetype GL - A C OpenGL Freetype engine
 *
 * Distributed under the OSI-approved BSD 2-Clause License.  See accompanying
 * file `LICENSE` for more details.
 */
#include <stdio.h>
#include <string.h>

/* Freetype GL - A C OpenGL Freetype engine
 *
 * Distributed under the OSI-approved BSD 2-Clause License.  See accompanying
 * file `LICENSE` for more details.
 */
#include <stdlib.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>

typedef union {
  float data[16]; /**< All compoments at once     */
  struct {
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;
  };
} mat4;

mat4 *mat4_new(void);

void mat4_set_identity(mat4 *self);

void mat4_set_zero(mat4 *self);

void mat4_multiply(mat4 *self, mat4 *other);

void mat4_set_orthographic(mat4 *self, float left, float right, float bottom,
                           float top, float znear, float zfar);

void mat4_set_perspective(mat4 *self, float fovy, float aspect, float zNear,
                          float zFar);

void mat4_set_frustum(mat4 *self, float left, float right, float bottom,
                      float top, float znear, float zfar);

void mat4_set_rotation(mat4 *self, float angle, float x, float y, float z);

void mat4_set_translation(mat4 *self, float x, float y, float z);

void mat4_set_scaling(mat4 *self, float x, float y, float z);

void mat4_rotate(mat4 *self, float angle, float x, float y, float z);

void mat4_translate(mat4 *self, float x, float y, float z);

void mat4_scale(mat4 *self, float x, float y, float z);

mat4 *mat4_new(void) {
  mat4 *self = (mat4 *)malloc(sizeof(mat4));
  return self;
}

void mat4_set_zero(mat4 *self) {
  if (!self)
    return;

  memset(self, 0, sizeof(mat4));
}

void mat4_set_identity(mat4 *self) {
  if (!self)
    return;

  memset(self, 0, sizeof(mat4));
  self->m00 = 1.0;
  self->m11 = 1.0;
  self->m22 = 1.0;
  self->m33 = 1.0;
}

void mat4_multiply(mat4 *self, mat4 *other) {
  mat4 m;
  size_t i;

  if (!self || !other)
    return;

  for (i = 0; i < 4; ++i) {
    m.data[i * 4 + 0] = (self->data[i * 4 + 0] * other->data[0 * 4 + 0]) +
                        (self->data[i * 4 + 1] * other->data[1 * 4 + 0]) +
                        (self->data[i * 4 + 2] * other->data[2 * 4 + 0]) +
                        (self->data[i * 4 + 3] * other->data[3 * 4 + 0]);

    m.data[i * 4 + 1] = (self->data[i * 4 + 0] * other->data[0 * 4 + 1]) +
                        (self->data[i * 4 + 1] * other->data[1 * 4 + 1]) +
                        (self->data[i * 4 + 2] * other->data[2 * 4 + 1]) +
                        (self->data[i * 4 + 3] * other->data[3 * 4 + 1]);

    m.data[i * 4 + 2] = (self->data[i * 4 + 0] * other->data[0 * 4 + 2]) +
                        (self->data[i * 4 + 1] * other->data[1 * 4 + 2]) +
                        (self->data[i * 4 + 2] * other->data[2 * 4 + 2]) +
                        (self->data[i * 4 + 3] * other->data[3 * 4 + 2]);

    m.data[i * 4 + 3] = (self->data[i * 4 + 0] * other->data[0 * 4 + 3]) +
                        (self->data[i * 4 + 1] * other->data[1 * 4 + 3]) +
                        (self->data[i * 4 + 2] * other->data[2 * 4 + 3]) +
                        (self->data[i * 4 + 3] * other->data[3 * 4 + 3]);
  }
  memcpy(self, &m, sizeof(mat4));
}

void mat4_set_orthographic(mat4 *self, float left, float right, float bottom,
                           float top, float znear, float zfar) {
  if (!self)
    return;

  if (left == right || bottom == top || znear == zfar)
    return;

  mat4_set_zero(self);

  self->m00 = +2.0f / (right - left);
  self->m30 = -(right + left) / (right - left);
  self->m11 = +2.0f / (top - bottom);
  self->m31 = -(top + bottom) / (top - bottom);
  self->m22 = -2.0f / (zfar - znear);
  self->m32 = -(zfar + znear) / (zfar - znear);
  self->m33 = 1.0f;
}

void mat4_set_perspective(mat4 *self, float fovy, float aspect, float znear,
                          float zfar) {
  float h, w;

  if (!self)
    return;

  if (znear == zfar)
    return;

  h = (float)tan(fovy / 360.0 * M_PI) * znear;
  w = h * aspect;

  mat4_set_frustum(self, -w, w, -h, h, znear, zfar);
}

void mat4_set_frustum(mat4 *self, float left, float right, float bottom,
                      float top, float znear, float zfar) {

  if (!self)
    return;

  if (left == right || bottom == top || znear == zfar)
    return;

  mat4_set_zero(self);

  self->m00 = (2.0f * znear) / (right - left);
  self->m20 = (right + left) / (right - left);

  self->m11 = (2.0f * znear) / (top - bottom);
  self->m21 = (top + bottom) / (top - bottom);

  self->m22 = -(zfar + znear) / (zfar - znear);
  self->m32 = -(2.0f * zfar * znear) / (zfar - znear);

  self->m23 = -1.0f;
}

void mat4_set_rotation(mat4 *self, float angle, float x, float y, float z) {
  float c, s, norm;

  if (!self)
    return;

  c = (float)cos(M_PI * angle / 180.0);
  s = (float)sin(M_PI * angle / 180.0);
  norm = (float)sqrt(x * x + y * y + z * z);

  x /= norm;
  y /= norm;
  z /= norm;

  mat4_set_identity(self);

  self->m00 = x * x * (1 - c) + c;
  self->m10 = y * x * (1 - c) - z * s;
  self->m20 = z * x * (1 - c) + y * s;

  self->m01 = x * y * (1 - c) + z * s;
  self->m11 = y * y * (1 - c) + c;
  self->m21 = z * y * (1 - c) - x * s;

  self->m02 = x * z * (1 - c) - y * s;
  self->m12 = y * z * (1 - c) + x * s;
  self->m22 = z * z * (1 - c) + c;
}

void mat4_set_translation(mat4 *self, float x, float y, float z) {
  if (!self)
    return;

  mat4_set_identity(self);
  self->m30 = x;
  self->m31 = y;
  self->m32 = z;
}

void mat4_set_scaling(mat4 *self, float x, float y, float z) {
  if (!self)
    return;

  mat4_set_identity(self);
  self->m00 = x;
  self->m11 = y;
  self->m22 = z;
}

void mat4_rotate(mat4 *self, float angle, float x, float y, float z) {
  mat4 m;

  if (!self)
    return;

  mat4_set_rotation(&m, angle, x, y, z);
  mat4_multiply(self, &m);
}

void mat4_translate(mat4 *self, float x, float y, float z) {
  mat4 m;

  if (!self)
    return;

  mat4_set_translation(&m, x, y, z);
  mat4_multiply(self, &m);
}

void mat4_scale(mat4 *self, float x, float y, float z) {
  mat4 m;

  if (!self)
    return;

  mat4_set_scaling(&m, x, y, z);
  mat4_multiply(self, &m);
}

#include "freetype-gl.h"
// ------------------------------------------------------------ shader_read ---
char *shader_read(const char *filename) {
  FILE *file;
  char *buffer;
  size_t size;

  file = fopen(filename, "rb");
  if (!file) {
    fprintf(stderr, "Unable to open file \"%s\".\n", filename);
    return 0;
  }
  fseek(file, 0, SEEK_END);
  size = ftell(file);
  fseek(file, 0, SEEK_SET);
  buffer = (char *)malloc((size + 1) * sizeof(char *));
  fread(buffer, sizeof(char), size, file);
  buffer[size] = 0;
  fclose(file);
}

// --------------------------------------------------------- shader_compile ---
GLuint shader_compile(const char *source, const GLenum type) {
  GLint compile_status;
  GLuint handle = glCreateShader(type);
  glShaderSource(handle, 1, &source, 0);
  glCompileShader(handle);

  glGetShaderiv(handle, GL_COMPILE_STATUS, &compile_status);
  if (compile_status == GL_FALSE) {
    GLchar messages[256];
    glGetShaderInfoLog(handle, sizeof(messages), 0, &messages[0]);
    fprintf(stderr, "%s\n", messages);
    exit(EXIT_FAILURE);
  }
  return handle;
}

// ------------------------------------------------------------ shader_load ---
GLuint shader_load(const char *vert_filename, const char *frag_filename) {
  GLuint handle = glCreateProgram();
  GLint link_status;

  if (vert_filename && strlen(vert_filename)) {
    char *vert_source = shader_read(vert_filename);
    GLuint vert_shader = shader_compile(vert_source, GL_VERTEX_SHADER);
    glAttachShader(handle, vert_shader);
    glDeleteShader(vert_shader);
    free(vert_source);
  }
  if (frag_filename && strlen(frag_filename)) {
    char *frag_source = shader_read(frag_filename);
    GLuint frag_shader = shader_compile(frag_source, GL_FRAGMENT_SHADER);
    glAttachShader(handle, frag_shader);
    glDeleteShader(frag_shader);
    free(frag_source);
  }

  glLinkProgram(handle);

  glGetProgramiv(handle, GL_LINK_STATUS, &link_status);
  if (link_status == GL_FALSE) {
    GLchar messages[256];
    glGetProgramInfoLog(handle, sizeof(messages), 0, &messages[0]);
    fprintf(stderr, "%s\n", messages);
    exit(1);
  }
  return handle;
}
#include "vertex-buffer.h"

#include <GLFW/glfw3.h>

// ------------------------------------------------------- typedef & struct ---
typedef struct {
  float x, y, z;    // position
  float s, t;       // texture
  float r, g, b, a; // color
} vertex_t;

// ------------------------------------------------------- global variables ---
GLuint shader;
texture_atlas_t *atlas;
vertex_buffer_t *buffer;
mat4 model, view, projection;

// --------------------------------------------------------------- add_text ---
void add_text(vertex_buffer_t *buffer, texture_font_t *font, char *text,
              vec4 *color, vec2 *pen) {
  size_t i;
  float r = color->red, g = color->green, b = color->blue, a = color->alpha;
  for (i = 0; i < strlen(text); ++i) {
    texture_glyph_t *glyph = texture_font_get_glyph(font, text + i);
    if (glyph != NULL) {
      float kerning = 0.0f;
      if (i > 0) {
        kerning = texture_glyph_get_kerning(glyph, text + i - 1);
      }
      pen->x += kerning;
      int x0 = (int)(pen->x + glyph->offset_x);
      int y0 = (int)(pen->y + glyph->offset_y);
      int x1 = (int)(x0 + glyph->width);
      int y1 = (int)(y0 - glyph->height);
      float s0 = glyph->s0;
      float t0 = glyph->t0;
      float s1 = glyph->s1;
      float t1 = glyph->t1;
      GLuint indices[6] = {0, 1, 2, 0, 2, 3};
      vertex_t vertices[4] = {{x0, y0, 0, s0, t0, r, g, b, a},
                              {x0, y1, 0, s0, t1, r, g, b, a},
                              {x1, y1, 0, s1, t1, r, g, b, a},
                              {x1, y0, 0, s1, t0, r, g, b, a}};
      vertex_buffer_push_back(buffer, vertices, 4, indices, 6);
      pen->x += glyph->advance_x;
    }
  }
}

// ------------------------------------------------------------------- init ---
void init(void) {
  size_t i;
  texture_font_t *font = 0;
  atlas = texture_atlas_new(512, 512, 1);
  const char *filename = "fonts/Vera.ttf";
  char *text = "A Quick Brown Fox Jumps Over The Lazy Dog 0123456789";
  buffer = vertex_buffer_new("vertex:3f,tex_coord:2f,color:4f");
  vec2 pen = {{5, 400}};
  vec4 black = {{0, 0, 0, 1}};
  for (i = 7; i < 27; ++i) {
    font = texture_font_new_from_file(atlas, i, filename);
    pen.x = 5;
    pen.y -= font->height;
    texture_font_load_glyphs(font, text);
    add_text(buffer, font, text, &black, &pen);
    texture_font_delete(font);
  }

  glGenTextures(1, &atlas->id);
  glBindTexture(GL_TEXTURE_2D, atlas->id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas->width, atlas->height, 0, GL_RED,
               GL_UNSIGNED_BYTE, atlas->data);

  shader = shader_load("shaders/v3f-t2f-c4f.vert", "shaders/v3f-t2f-c4f.frag");
  mat4_set_identity(&projection);
  mat4_set_identity(&model);
  mat4_set_identity(&view);
}

// ---------------------------------------------------------------- display ---
void display(GLFWwindow *window) {
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glUseProgram(shader);
  {
    glUniform1i(glGetUniformLocation(shader, "texture"), 0);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, 0, model.data);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, 0, view.data);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, 0,
                       projection.data);
    vertex_buffer_render(buffer, GL_TRIANGLES);
  }

  glfwSwapBuffers(window);
}

// ---------------------------------------------------------------- reshape ---
void reshape(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  mat4_set_orthographic(&projection, 0, width, 0, height, -1, 1);
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

  window = glfwCreateWindow(800, 500, argv[0], NULL, NULL);

  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glfwSetFramebufferSizeCallback(window, reshape);
  glfwSetWindowRefreshCallback(window, display);
  glfwSetKeyCallback(window, keyboard);

#ifndef __APPLE__
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    /* Problem: glewInit failed, something is seriously wrong. */
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    exit(EXIT_FAILURE);
  }
  fprintf(stderr, "Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

  init();

  glfwShowWindow(window);
  reshape(window, 800, 500);

  while (!glfwWindowShouldClose(window)) {
    display(window);
    glfwPollEvents();
  }

  glDeleteTextures(1, &atlas->id);
  atlas->id = 0;
  texture_atlas_delete(atlas);

  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}
