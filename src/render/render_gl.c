#include "log.h"
#include "render/font/font.h"
#include "render/render.h"
#include "render/shader.h"

#include "cglm/call.h"

/*
 * source from https://learnopengl.com/
 * basic test to see if rendering works
 */

static const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"
    "uniform mat4 projection;\n"
    "void main(){\n"
    //"    gl_Position = vec4(aPos, 1.0);\n"
    "    gl_Position = projection * vec4(aPos, 1.0);\n"
    "    ourColor = aColor;\n"
    "    TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
    "}\0";

static const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D texture1;\n"
    "void main(){\n"
    "    FragColor = vec4(ourColor, 1.0) * texture(texture1, TexCoord).r;\n"
    "}\0";

static unsigned int shaderProgram;
static unsigned int VBO, EBO, VAO;
static unsigned int texture;

static int init() {
  texture = fttest();
  log_debug("gl render init");

  log_info("compiling shader");
  shaderProgram = shader_load(vertexShaderSource, fragmentShaderSource);

  log_info("loading vertex buffer");
  float vertices[] = {
      // positions        // colors         // texture coords
      500, 500, 1, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top right
      500, 0,   1, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // bottom right
      0,   0,   1, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // bottom left
      0,   500, 1, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f  // top left

  };
  unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glUseProgram(shaderProgram);
  glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

  mat4 projection = GLM_MAT4_IDENTITY_INIT;
  glmc_ortho(0, 500, 0, 500, -1, 1, projection);
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, 0,
                     (float *)projection);

  return 0;
}

static int draw() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  return 0;
}

render_interface render_gl = {&init, &draw};
