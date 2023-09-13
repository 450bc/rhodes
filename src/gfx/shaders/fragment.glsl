#version 460 core

out vec4 FragColor; // output color to screen

in vec3 ourColor; // input color from vertex shader

void main()
{
    FragColor = vec4(ourColor, 1.0); // set output color to input color
}