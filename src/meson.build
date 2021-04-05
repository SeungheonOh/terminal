cc = meson.get_compiler('c')
subdir('render')
subdir('util')

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

  dependency('glfw3', fallback: ['glfw']),

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
