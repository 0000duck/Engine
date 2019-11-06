#ifndef __ModuleModelLoader_h__
#define __ModuleModelLoader_h__

#include "Module.h"

#include "MathGeoLib.h"

#include <vector>
#include <string>

struct aiScene;
struct par_shapes_mesh_s;

class ModuleModelLoader : public Module
{
public:

    ModuleModelLoader();
    ~ModuleModelLoader();

	bool            Init            () override;
	update_status   Update          () override;
	bool            CleanUp         () override;

    bool            Load            (const char* file);

    bool            LoadSphere      (const char* name, const math::float3& pos, const math::Quat& rot, float size, 
                                    unsigned slices, unsigned stacks, const math::float4& color);

    bool            LoadCylinder    (const char* name, const math::float3& pos, const math::Quat& rot, float height, 
                                    float radius, unsigned slices, unsigned stacks, const math::float4& color);

    bool            LoadTorus       (const char* name, const math::float3& pos, const math::Quat& rot, float inner_r, float outer_r, 
                                    unsigned slices, unsigned stacks, const math::float4& color);

    bool            LoadCube        (const char* name, const math::float3& pos, const math::Quat& rot, float size, const math::float4& color);

    void            Clear       ();

public:

    struct Mesh
    {
        std::string     name;
        unsigned        vertex_size      = 0;
        unsigned        vao              = 0;
        unsigned        vbo              = 0;
        unsigned        ibo              = 0;
        unsigned        num_vertices     = 0;
        unsigned        num_indices      = 0;
        unsigned        texcoords_offset = 0;
        unsigned        normals_offset   = 0;
        unsigned        material         = 0;
        math::float4x4  transform        = math::float4x4::identity;
    };

    struct Material
    {
        unsigned program             = 0;
        unsigned diffuse_map         = 0;
        math::float4 object_color    = math::float4::zero;
        float shininess              = 0.0f;
        float k_specular             = 0.0f;
        float k_diffuse              = 1.0f;
        float k_ambient              = 0.0f;
    };

    struct Sphere
    {
        math::float3 center = math::float3::zero;
        float        radius = 0.0f;
    };

    struct Light
    {
        math::float4 color = math::float4::one;
        math::float3 pos   = math::float3::one;
    };


private:

    void            GenerateMesh        (const char* name, const math::float3& pos, const math::Quat& rot, par_shapes_mesh_s* shape);
    void            GenerateMeshes      (const aiScene* scene);
    void            GenerateVAO         (Mesh& mesh);
    void            GenerateMaterials   (const aiScene* scene);
    void            ComputeBoundSphere  (math::float3& center, float radius);

public:

    std::vector<Mesh>     meshes;
    std::vector<Material> materials;
    Sphere                bsphere;
    Light                 light;
    float			      ambient = 0.0f;

private:

	math::float3          min_v = math::float3(FLT_MAX, FLT_MAX, FLT_MAX);
	math::float3          max_v = math::float3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
};

#endif // __ModuleModelLoader_h__ 

