#version 450

in vec3 position;
in vec3 normal;

out vec3 normal_out;
out vec3 position_out;

uniform mat4 transformation;

void main()
{
    gl_Position = transformation * vec4(position, 1.0);
    normal_out = normal;
    position_out = position;
}
