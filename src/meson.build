cc = meson.get_compiler('c')

# freetype-gl
#add_project_arguments('-DFREETYPE_GL_USE_VAO', language : 'c')
#add_project_arguments('-DGL_WITH_GLAD', language : 'c')
freetype_gl_incdir = include_directories('freetype-gl')
freetype_gl_sources = files(
    'freetype-gl/distance-field.c',
    'freetype-gl/edtaa3func.c',
    'freetype-gl/font-manager.c',
    'freetype-gl/platform.c',
    'freetype-gl/text-buffer.c',
    'freetype-gl/texture-atlas.c',
    'freetype-gl/texture-font.c',
    'freetype-gl/vector.c',
    'freetype-gl/vertex-attribute.c',
    'freetype-gl/vertex-buffer.c',
    'freetype-gl/utf8-utils.c',
)
freetype_gl_deps = [
    dependency('glfw3', fallback: ['glfw']),
    dependency('fontconfig'),
    dependency('freetype2'),
    dependency('harfbuzz'),
]

freetype_gl = static_library('freetype-gl',
    sources: freetype_gl_sources,
    include_directories: freetype_gl_incdir,
    dependencies: freetype_gl_deps)

freetype_gl_dep = declare_dependency(
    include_directories : freetype_gl_incdir,
    link_with : freetype_gl,
    sources : freetype_gl_sources)

# You need linkder for linux
if host_machine.system() == 'linux'
    dl_dep = cc.find_library('dl', required: false)
else
    dl_dep = dependency('', required: false)
endif

deps = [
     dl_dep,
     dependency('glfw3'),
     dependency('gl'),
     freetype_gl_deps,
     freetype_gl_dep,
]

inc = [
    include_directories('/usr/local/include/glad'),
    include_directories('../include'),
    freetype_gl_incdir,
]

src = [
    'test.c',
    'log.c',
    'shader.c',
    'glad.c',
    'mat4.c',
]

executable('term', sources: src,
		   include_directories: inc,
		   dependencies: deps)
