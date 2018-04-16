#version 330 core
in vec3 result;
out vec4 FragColor;

uniform vec3 objectColor;

void main()
{	
    FragColor = vec4(result * objectColor, 1.0);
}