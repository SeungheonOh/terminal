#include "log.h"
#include "render/font/font.h"
#include "render/render.h"
#include "render/shader.h"

/*
 * source from https://learnopengl.com/
 * basic test to see if rendering works
 */

static const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
static const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

static int counter;
static unsigned int shaderProgram;
static unsigned int VBO, VAO;

static int init() {
  fttest();
  counter = 0;
  log_debug("gl render init");

  log_info("compiling shader");
  shaderProgram = shader_load(vertexShaderSource, fragmentShaderSource);

  log_info("loading vertex buffer");
  float vertices[] = {
      -0.5f, -0.5f, 0.0f, // left
      0.5f,  -0.5f, 0.0f, // right
      0.0f,  0.5f,  0.0f  // top
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  return 0;
}

static int draw() {
  // log_debug("gl render draw for %d time", ++counter);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // draw our first triangle
  glUseProgram(shaderProgram);
  glBindVertexArray(
      VAO); // seeing as we only have a single VAO there's no need to bind it
            // every time, but we'll do so to keep things a bit more organized
  glDrawArrays(GL_TRIANGLES, 0, 3);
  return 0;
}

render_interface render_gl = {&init, &draw};
