#version 330 core

out vec4 color;

in vec2 uv0;

uniform sampler2D diffuse_map;
uniform vec4 diffuse_color;
uniform int use_diffuse_texture;

void main()
{
    if(use_diffuse_texture == 1)
    {
        color = texture2D(diffuse_map, uv0);
    }
    else
    {
        color = diffuse_color;
    }
}
