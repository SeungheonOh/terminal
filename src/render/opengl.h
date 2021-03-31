#if defined(__APPLE__)
# pragma clang diagnostic ignored "-Wdeprecated-declarations"
//# include <OpenGL/gl.h>
# define GLEW_STATIC
# include <GL/glew.h>
#elif
# include <GL/glew.h>
# include <GL/gl.h>
#endif
