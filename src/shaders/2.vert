uniform sampler2D tex;
uniform vec3 pixel;

attribute vec3 vertex;
attribute vec4 color;
attribute vec2 tex_coord;
attribute float ashift;
attribute float agamma;

varying vec4 vcolor;
varying vec2 vtex_coord;
varying float vshift;
varying float vgamma;

void main()
{
    vshift = ashift;
    vgamma = agamma;
    vcolor = color;
    vtex_coord = tex_coord;
    gl_Position = vec4(vertex, 1.0);
}
