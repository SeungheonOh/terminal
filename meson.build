project('terminal', 'c')

subdir('src')
subdir('test')

e = executable('term', sources: src,
		   include_directories: inc,
		   dependencies: deps)

test('run', e)
test('util_test', util_test)
