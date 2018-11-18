#version 330 core

#define SHOW_ALL 0
#define SHOW_AMBIENT 1
#define SHOW_DIFFUSE 2
#define SHOW_SPECULAR 3

uniform sampler2D diffuse_map;
uniform vec4 diffuse_color;
uniform int use_diffuse_map;
uniform mat4 view;

uniform vec3 light_pos;
uniform float ambient;
uniform float shininess;
uniform float glossiness;
uniform int show_type;

in vec3 normal;
in vec3 position;
in vec2 uv0;

out vec4 color;

void main()
{
    vec3 normal      = normalize(normal);
    vec3 light_dir   = normalize(light_pos-position);
    float diffuse    = max(0.0, dot(normal, light_dir));
    float specular   = 0.0;

    if(diffuse > 0.0 && glossiness > 0.0 && shininess > 0.0)
    {
        vec3 view_pos    = transpose(mat3(view))*(-view[3].xyz);
        vec3 view_dir    = normalize(view_pos-position);
        vec3 half_dir    = normalize(view_dir+light_dir);
        float sp         = max(dot(normal, half_dir), 0.0);

        if(sp > 0.0)
        {
            specular = glossiness*pow(sp, shininess); 
        }
    }
    
    float intensity = 0.0;

    if(show_type == SHOW_AMBIENT)
    {
        intensity = ambient;
    }
    else if(show_type == SHOW_DIFFUSE)
    {
        intensity = diffuse;
    }
    else if(show_type == SHOW_SPECULAR)
    {
        intensity = specular;
    }
    else
    {
        intensity = (ambient+diffuse+specular); 
    }

    if(use_diffuse_map == 1)
    {
        color = texture2D(diffuse_map, uv0);
    }
    else
    {
        color = diffuse_color;
    }

    color = vec4(color.rgb*intensity, color.a);

}
