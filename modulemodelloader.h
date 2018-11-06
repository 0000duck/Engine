#ifndef __ModuleModelLoader_h__
#define __ModuleModelLoader_h__

#include "Module.h"

#include "MathGeoLib.h"

#include <vector>

struct aiScene;

class ModuleModelLoader : public Module
{
public:

    ModuleModelLoader();
    ~ModuleModelLoader();

	bool            Init    ();
	update_status   Update  ();
	bool            CleanUp ();

private:

    void            GenerateMeshes(const aiScene* scene);
    void            GenerateMaterials(const aiScene* scene);
    void            ComputeBoundSphere(math::float3& center, float radius);

public:

	enum VertexAttribs
	{
		ATTRIB_TEX_COORDS_0 = 1 << 0,
		ATTRIB_NORMALS      = 1 << 1,
        ATTRIB_TANGENTS     = 1 << 2,
		ATTRIB_BONES        = 1 << 3,
	};

    struct Mesh
    {
        unsigned attribs      = 0;
        unsigned vertex_size  = 0;
        unsigned vao          = 0;
        unsigned vbo          = 0;
        unsigned ibo          = 0;
        unsigned material     = 0;
        unsigned num_vertices = 0;
        unsigned num_indices  = 0;
    };

    struct Material
    {
        unsigned texture0 = 0;
    };

    struct Sphere
    {
        math::float3 center = math::float3::zero;
        float        radius = 0.0f;
    };

    std::vector<Mesh>     meshes;
    std::vector<Material> materials;
    math::float4x4        transform = math::float4x4::identity;
    Sphere                bsphere;
};

#endif // __ModuleModelLoader_h__ 

