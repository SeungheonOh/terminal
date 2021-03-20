#include "render/shader.h"

char *shader_read_file(const char *file) {
  FILE *fp;
  long lSize;
  char *buffer;

  fp = fopen(file, "rb");
  if (!fp) {
    log_error("failed to open file: %s", file);
    return 0;
  }

  fseek(fp, 0L, SEEK_END);
  lSize = ftell(fp);
  rewind(fp);

  /* allocate memory for entire content */
  buffer = calloc(1, lSize + 1);
  if (!buffer) {
    fclose(fp);
    log_error("memory allocation failed");
    return 0;
  }

  /* copy the file into the buffer */
  if (1 != fread(buffer, lSize, 1, fp)) {
    fclose(fp);
    free(buffer);
    log_error("entire read fails");
    return 0;
  }
  buffer[lSize] = 0;
  return buffer;
}

GLuint shader_compile(const char *shader_source, GLuint type) {
  int success;
  char info[512];
  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &shader_source, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, info);
    log_error("shader compilation failed: \n%s", info);
    return -1;
  }
  return shader;
}

GLuint shader_load(const char *vert, const char *frag) {
  if (!vert || !strlen(vert) || !frag || !strlen(frag)) {
    log_error("invalid shader strings");
    exit(EXIT_FAILURE);
  }
  int success;
  char info[512];
  GLuint shaderprog = glCreateProgram();

  GLuint vertshader = shader_compile(vert, GL_VERTEX_SHADER);
  if (vertshader == -1) {
    log_error("Failed to load vertex shader");
    exit(EXIT_FAILURE);
  }
  glAttachShader(shaderprog, vertshader);
  glDeleteShader(vertshader);

  GLuint fragshader = shader_compile(frag, GL_FRAGMENT_SHADER);
  if (fragshader == -1) {
    log_error("Failed to load fragment shader");
    exit(EXIT_FAILURE);
  }
  glAttachShader(shaderprog, fragshader);
  glDeleteShader(fragshader);

  glLinkProgram(shaderprog);

  glGetProgramiv(shaderprog, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderprog, 512, NULL, info);
    log_error("failed to load shader: %s", info);
    exit(EXIT_FAILURE);
  }

  return shaderprog;
}

GLuint shader_load_file(const char *vert, const char *frag) {
  int success;
  char info[512];
  GLuint shaderprog = glCreateProgram();
  if (vert && strlen(vert)) {
    char *vertsrc = shader_read_file(vert);
    GLuint vertshader = shader_compile(vertsrc, GL_VERTEX_SHADER);
    glAttachShader(shaderprog, vertshader);
    glDeleteShader(vertshader);
    free(vertsrc);
  }
  if (frag && strlen(frag)) {
    char *fragsrc = shader_read_file(vert);
    GLuint fragshader = shader_compile(fragsrc, GL_FRAGMENT_SHADER);
    glAttachShader(shaderprog, fragshader);
    glDeleteShader(fragshader);
    free(fragsrc);
  }

  glLinkProgram(shaderprog);

  glGetProgramiv(shaderprog, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderprog, 512, NULL, info);
    log_error("failed to load shader: %s", info);
    exit(EXIT_FAILURE);
  }

  return shaderprog;
}
