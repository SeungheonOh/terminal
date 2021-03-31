#if defined(__APPLE__)
# pragma clang diagnostic ignored "-Wdeprecated-declarations"
# define GLEW_STATIC
# include <GL/glew.h>
#else
# include <GL/glew.h>
# include <GL/gl.h>
#endif
