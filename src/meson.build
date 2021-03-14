cc = meson.get_compiler('c')

# cglm - opengl matrix library
#cglm_proj = subproject('cglm')
#cglm_deps = cglm_proj.get_variable('cglm_dep')

# freetype-gl - freetype opengl rendering library
# not going to use, no color font support
#cmake = import('cmake')
#freetypegl_proj = cmake.subproject('freetype-gl')
#freetypegl_deps = freetypegl_proj.dependency('freetype-gl')

subdir('render')

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

  #subdir
  render_dep,

  dependency('glfw3', fallback: ['glfw']),
]

inc = [
  include_directories('.'),
  include_directories('../subprojects'),
]

src = [
  'main.c',
  'log.c',
  #'shader.c',
  #'mat4.c',
  render_src,
]

e = executable('term', sources: src,
		   include_directories: inc,
		   dependencies: deps)

test('run', e)
