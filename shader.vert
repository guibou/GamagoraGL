#version 450

in vec4 position;
in vec4 color;

out vec4 c;

void main()
{
    gl_Position = vec4(position);
    c = vec4(color);
}