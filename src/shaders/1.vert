uniform vec4 Color;
attribute vec3 vertex;
attribute vec4 color;
void main()
{
    gl_FrontColor = color*Color;
    gl_Position = vec4(vertex, 1.0);
}