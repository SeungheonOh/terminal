cc = meson.get_compiler('c')
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
  files('main.c'),
  files('log.c'),
  render_src,
]
