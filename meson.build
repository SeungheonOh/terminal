project('terminal', 'c')

subdir('src')

e = executable('term', sources: src,
		   include_directories: inc,
		   dependencies: deps)

test('run', e)
