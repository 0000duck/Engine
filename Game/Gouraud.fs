#version 330 core

uniform vec4 diffuse_color;

out vec4 color;

in vec2 uv0;
in float intensity;


void main()
{
    color = vec4(intensity*diffuse_color.x, intensity*diffuse_color.y, intensity*diffuse_color.z, 1.0);
}
