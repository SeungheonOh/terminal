cc = meson.get_compiler('c')
subdir('render')
subdir('util')

deps = [
  # General
  cc.find_library('m', required: true),

  dependency('glfw3', fallback: ['glfw']),
  dependency('opengl'),

  #subdir
  render_dep,
  util_dep
]

inc = [
  include_directories('.'),
  include_directories('../subprojects'),

  render_inc,
  util_inc,
]

src = [
  files('main.c'),
  files('log.c'),

  render_src,
  util_src,
]

# You need linkder for linux
if host_machine.system() == 'linux'
  deps += cc.find_library('dl', required: false)
endif

if host_machine.system() == 'darwin'
  deps += dependency('opengl')
endif
