# cglm - opengl matrix library
cglm_proj = subproject('cglm')
cglm_deps = cglm_proj.get_variable('cglm_dep')

render_dep = [
  cglm_deps,

  dependency('glew'),
  dependency('fontconfig'),
  dependency('freetype2'),
]

render_src = [
  files('render_gl.c'),
  files('shader.c'),
]

render_inc = [
  include_directories('.'),
]
