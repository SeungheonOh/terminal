#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/gl.h>

#include "log.h"

char* shader_read_file(const char *);
GLuint shader_compile(const char*, GLuint);
GLuint shader_load(const char*, const char*);
GLuint shader_load_file(const char*, const char*);
