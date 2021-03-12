#include <cglm/cglm.h>


int main() {
  mat4 dest;
  glm_mat4_copy(GLM_MAT4_IDENTITY, dest);
  glm_rotate(dest, drand48(), (vec3){drand48(), drand48(), drand48()});

  return 0;
}
