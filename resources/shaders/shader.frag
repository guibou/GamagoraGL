#version 450

in vec3 normal_out;

out vec4 color;

uniform vec3 Le;
uniform vec3 positionLight;
uniform vec3 albedo;

void main()
{
    // color = vec4(vec3(1, 1, 1), 1.0);
    // Display the normal:
    color = vec4(abs(normal_out), 1.0);
}
