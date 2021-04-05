# cglm - opengl matrix library
cglm_proj = subproject('cglm')
cglm_dep = cglm_proj.get_variable('cglm_dep')

subdir('font')

render_dep = [
  cglm_dep,
  font_dep,

  dependency('glew'),
  #  dependency('opengl'),
]

render_src = [
  font_src,
  files('render_gl.c'),
  files('shader.c'),
]

render_inc = [
  font_inc,
  include_directories('.'),
]
