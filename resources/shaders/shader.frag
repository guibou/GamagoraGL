#version 450

in vec3 normal_out;
in vec3 position_out;

out vec4 color;

uniform vec3 Le;
uniform vec3 positionLight;
uniform vec3 albedo;

void main()
{
    // color = vec4(vec3(1, 1, 1), 1.0);
    // Display the normal:
    vec3 directionToLight = positionLight - position_out;
    float d2 = dot(directionToLight, directionToLight);
    vec3 omegaI = normalize(directionToLight);
    //
    //
    color = vec4(Le / d2 * dot(normal_out, omegaI) * albedo, 1.0);
}
