#version 450

layout(local_size_x = 32) in;

uniform vec2 mouse;

layout(std430) struct particule
{
    vec4 position;
    vec4 color;
    vec4 speed;
};

layout(std430, binding = 0) buffer parts
{
    particule particules[];
};

void main()
{
    float dt = 0.01;

    particules[gl_GlobalInvocationID.x].speed.w += dt;

    if(particules[gl_GlobalInvocationID.x].speed.w > 5)
    {
	particules[gl_GlobalInvocationID.x].position = vec4(mouse * vec2(1, -1), 0, 1);
float	w = particules[gl_GlobalInvocationID.x].speed.w;
        particules[gl_GlobalInvocationID.x].speed.w = 0;

	particules[gl_GlobalInvocationID.x].speed.xyz = vec3((w - 5) * 20, (w - 5) * 100, 0);
    }
    else
    {
	particules[gl_GlobalInvocationID.x].position.xyz += dt * particules[gl_GlobalInvocationID.x].speed.xyz;
	particules[gl_GlobalInvocationID.x].speed.xyz += dt * vec3(0, -0.5, 0);
    }




    //particules[gl_GlobalInvocationID.x].position = vec4(mouse * vec2(1, -1), 0, 1);
}