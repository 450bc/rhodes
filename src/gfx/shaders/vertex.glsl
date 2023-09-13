#version 460 core

layout (location = 0) in vec3 aPos; // vertex position attribute
layout (location = 1) in vec3 aColor; // vertex color attribute

out vec3 ourColor; // output color to fragment shader

uniform mat4 model; // model matrix uniform
uniform mat4 view; // view matrix uniform
uniform mat4 projection; // projection matrix uniform

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0); // transform vertex position to clip space
    ourColor = aColor; // pass color to fragment shader
}