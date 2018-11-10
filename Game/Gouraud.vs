#version 330 

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv0;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

uniform vec3  light_pos;
uniform float ambient;
uniform float shininess;
uniform float glossiness;

out vec2 uv0;
out float intensity;

void main()
{
    vec3 position    = (model*vec4(vertex_position, 1.0)).xyz;
    vec3 normal      = (model*vec4(vertex_normal, 0.0)).xyz;
    gl_Position      = proj*view*vec4(position, 1.0);
    uv0              = vertex_uv0;

    vec3 light_dir   = normalize(light_pos-position);
    float diffuse    = max(0.0, dot(normal, light_dir));
    float specular   = 0.0;

    if(diffuse > 0.0 && glossiness > 0.0 && shininess > 0.0)
    {
        vec3 view_pos    = transpose(mat3(view))*(-view[3].xyz);
        vec3 view_dir    = normalize(view_pos-position);
        vec3 reflect_dir = reflect(-light_dir, normal);
        float sp         = max(dot(view_dir, reflect_dir), 0.0);

        if(sp > 0.0)
        {
            specular = glossiness*pow(sp, shininess); 
        }
    }
    
	intensity = (ambient+diffuse+specular)/(ambient+2.0);
}
