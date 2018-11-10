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

	bool            Init        ();
	update_status   Update      ();
	bool            CleanUp     ();

    bool            Load        (const char* file);
    bool            LoadSphere  (const char* name, float size, unsigned slices, unsigned stacks, const math::float4& color);
    void            Clear       ();

public:

    struct Mesh
    {
        std::string name;
        unsigned    vertex_size      = 0;
        unsigned    vao              = 0;
        unsigned    vbo              = 0;
        unsigned    ibo              = 0;
        unsigned    material         = 0;
        unsigned    num_vertices     = 0;
        unsigned    num_indices      = 0;
        unsigned    texcoords_offset = 0;
        unsigned    normals_offset   = 0;
    };

    struct Material
    {
        unsigned program           = 0;
        unsigned diffuse_map       = 0;
        math::float4 diffuse_color = math::float4::zero;
        float shininess            = 0.0f;
        float glossiness           = 0.0f;
    };

    struct Sphere
    {
        math::float3 center = math::float3::zero;
        float        radius = 0.0f;
    };

    struct Light
    {
        math::float4 color = math::float4::one;
        math::float3 pos   = math::float3::zero;
    };


private:

    void            GenerateMesh        (const char* name, par_shapes_mesh_s* shape);
    void            GenerateMeshes      (const aiScene* scene);
    void            GenerateVAO         (Mesh& mesh);
    void            GenerateMaterials   (const aiScene* scene);
    void            ComputeBoundSphere  (math::float3& center, float radius);

public:

    std::vector<Mesh>     meshes;
    std::vector<Material> materials;
    math::float4x4        transform = math::float4x4::identity;
    Sphere                bsphere;
    Light                 light;
    float			      ambient = 0.0f;
};

#endif // __ModuleModelLoader_h__ 

