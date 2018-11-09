#version 330 core

out vec4 color;

in vec2 uv0;
in float intensity;

void main()
{
    color = vec4(intensity, intensity, intensity, 1.0);
}
