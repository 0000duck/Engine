#version 330 core

#define SHOW_ALL 0
#define SHOW_AMBIENT 1
#define SHOW_DIFFUSE 2
#define SHOW_SPECULAR 3

uniform vec4 diffuse_color;
uniform mat4 view;

uniform vec3 light_pos;
uniform float ambient;
uniform float shininess;
uniform float glossiness;
uniform int show_type;

in vec2 uv0;
in vec3 normal;
in vec3 position;

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
        vec3 reflect_dir = normalize(reflect(-light_dir, normal));
        float sp         = max(dot(view_dir, reflect_dir), 0.0);

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
    color = vec4(intensity*diffuse_color.x, intensity*diffuse_color.y, intensity*diffuse_color.z, 1.0);
}
