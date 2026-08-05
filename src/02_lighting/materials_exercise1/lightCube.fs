#version 330 core
in vec3 Normal;
out vec4 FragColor;

uniform vec3 lightCubeColor;

void main()
{
    FragColor = vec4(lightCubeColor, 1.0);
}
