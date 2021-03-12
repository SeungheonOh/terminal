cc = meson.get_compiler('c')

# cglm - opengl matrix library
cglm_proj = subproject('cglm')
cglm_deps = cglm_proj.get_variable('cglm_dep')

# freetype-gl - freetype opengl rendering library
cmake = import('cmake')
freetypegl_proj = cmake.subproject('freetype-gl')
freetypegl_deps = freetypegl_proj.dependency('freetype-gl')

# You need linkder for linux
if host_machine.system() == 'linux'
  dl_dep = cc.find_library('dl', required: false)
else
  dl_dep = dependency('', required: false)
endif

deps = [
  # General
  dl_dep,
  cc.find_library('m', required: true),

  # freetype-gl; need to add dependencies here because cmake
  dependency('glew'),
  dependency('fontconfig'),
  dependency('freetype2'),
  dependency('harfbuzz'),
  freetypegl_deps,

  # cglm
  cglm_deps,

  dependency('glfw3', fallback: ['glfw']),
]

inc = [
  '../subprojects',
]

src = [
    'test.c',
    'log.c',
    'shader.c',
    'mat4.c',
]

executable('term', sources: src,
		   include_directories: inc,
		   dependencies: deps)
