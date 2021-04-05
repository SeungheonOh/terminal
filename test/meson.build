util_dep = [
]

util_src = [
  files('util_test.c'),

  files('../src/util/list.c'),
  files('../src/log.c'),
]

util_inc = [
  include_directories('../src'),
  include_directories('../src/util'),

  inc,
]

util_test = executable('util_test', sources: util_src,
      include_directories: util_inc,
      dependencies: util_dep)
