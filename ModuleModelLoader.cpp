#include "Globals.h"

#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleTextures.h"
#include "ModulePrograms.h"

#pragma warning(push)
#pragma warning(disable : 4996)  
#pragma warning(disable : 4244)  
#pragma warning(disable : 4305)  
#pragma warning(disable : 4838)  

#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"
#pragma warning(pop)

#include "GL/glew.h"

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>


ModuleModelLoader::ModuleModelLoader()
{
}

ModuleModelLoader::~ModuleModelLoader()
{
}

bool ModuleModelLoader::Init()
{
    // initial setup
    
    LoadSphere("light", 0.1f, 10, 10, math::float4(1.0f, 1.0f, 1.0f, 1.0f));
    light_mesh = meshes.back();
    light_material = materials.back();

    meshes.clear();
    materials.clear();

    return true;
}

update_status ModuleModelLoader::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleModelLoader::CleanUp()
{
    Clear();

    if(light_mesh.vao != 0)
    {
        glDeleteVertexArrays(1, &light_mesh.vao);
    }

    if (light_mesh.vbo != 0)
    {
        glDeleteBuffers(1, &light_mesh.vbo);
    }

    if (light_mesh.ibo != 0)
    {
        glDeleteBuffers(1, &light_mesh.ibo);
    }

    return true;
}

bool ModuleModelLoader::Load(const char* file)
{
    Clear();

	const aiScene* scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);

    if(scene)
    {
        GenerateMeshes(scene);
        GenerateMaterials(scene);
		
		aiReleaseImport(scene);

        return true;
    }

	LOG("Error loading %s: %s", file, aiGetErrorString());

    return false;
}

bool ModuleModelLoader::LoadSphere(const char* name, float size, unsigned slices, unsigned stacks, const math::float4& color)
{
    Clear();

    par_shapes_mesh* mesh = par_shapes_create_parametric_sphere(int(slices), int(stacks));

	if (mesh)
	{
        par_shapes_scale(mesh, size, size, size);

		GenerateMesh(name, mesh);
		par_shapes_free_mesh(mesh);

        Material mat;
        mat.program		  = ModulePrograms::DEFAULT_PROGRAM;
        mat.diffuse_color = color;

        materials.push_back(mat);

		return true;
	}

	return false;
}

void ModuleModelLoader::GenerateMesh(const char* name, par_shapes_mesh* shape)
{
	math::float3 min_v(FLT_MAX, FLT_MAX, FLT_MAX);
	math::float3 max_v(-FLT_MAX, -FLT_MAX, -FLT_MAX);


    Mesh dst_mesh;

    dst_mesh.name = name;

    glGenBuffers(1, &dst_mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, dst_mesh.vbo);

    // Positions

    for(unsigned i=0; i< unsigned(shape->npoints); ++i)
    {
        for(unsigned j=0; j<3; ++j)
        {
            min_v[j] = min(min_v[j], shape->points[i*3+j]);
            max_v[j] = max(max_v[j], shape->points[i*3+j]);
        }
    }

    unsigned offset_acc  = sizeof(math::float3);

    if(shape->normals)
    {
        dst_mesh.normals_offset = offset_acc;
        offset_acc += sizeof(math::float3);
    }

    if(shape->tcoords)
    {
        dst_mesh.texcoords_offset = offset_acc;
        offset_acc += sizeof(math::float2);
    }

    dst_mesh.vertex_size = offset_acc;

    glBufferData(GL_ARRAY_BUFFER, dst_mesh.vertex_size*shape->npoints, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(math::float3)*shape->npoints, shape->points);

    // normals
    
    if(shape->normals)
    {
        glBufferSubData(GL_ARRAY_BUFFER, dst_mesh.normals_offset*shape->npoints, sizeof(math::float3)*shape->npoints, shape->normals);
    }

    // tcoords
    
    if(shape->tcoords)
    {
        glBufferSubData(GL_ARRAY_BUFFER, dst_mesh.texcoords_offset*shape->npoints, sizeof(math::float2)*shape->npoints, shape->tcoords);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // indices

    glGenBuffers(1, &dst_mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst_mesh.ibo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*shape->ntriangles*3, nullptr, GL_STATIC_DRAW);

    unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, 
            sizeof(unsigned)*shape->ntriangles*3, GL_MAP_WRITE_BIT);

    for(unsigned i=0; i< unsigned(shape->ntriangles*3); ++i)
    {
        *(indices++) = shape->triangles[i];
    }

    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    dst_mesh.material	  = 0;
    dst_mesh.num_vertices = shape->npoints;
    dst_mesh.num_indices  = shape->ntriangles*3;


    GenerateVAO(dst_mesh);

    meshes.push_back(dst_mesh);

    bsphere.center = (max_v+min_v)*0.5f;
    bsphere.radius = (max_v-min_v).Length()*0.5f;
}

void ModuleModelLoader::GenerateVAO(Mesh& mesh)
{
    glGenVertexArrays(1, &mesh.vao);

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    if(mesh.normals_offset != 0)
    {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.normals_offset*mesh.num_vertices));
    }

    if(mesh.texcoords_offset != 0)
    {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.texcoords_offset*mesh.num_vertices));
    }

    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModuleModelLoader::Clear()
{
    for (unsigned i = 0; i < meshes.size(); ++i)
    {
        if(meshes[i].vao != 0)
        {
            glDeleteVertexArrays(1, &meshes[i].vao);
        }

        if (meshes[i].vbo != 0)
        {
            glDeleteBuffers(1, &meshes[i].vbo);
        }

        if (meshes[i].ibo != 0)
        {
            glDeleteBuffers(1, &meshes[i].ibo);
        }
    }

    for (unsigned i = 0; i < materials.size(); ++i)
    {
        if (materials[i].diffuse_map != 0)
        {
            App->textures->Unload(materials[i].diffuse_map);
        }
    }

    meshes.clear();
    materials.clear();
}

void ModuleModelLoader::GenerateMeshes(const aiScene* scene)
{
    math::float3 min_v(FLT_MAX, FLT_MAX, FLT_MAX);
    math::float3 max_v(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for(unsigned i=0; i< scene->mNumMeshes; ++i)
    {
        const aiMesh* src_mesh = scene->mMeshes[i];

        Mesh dst_mesh;

        dst_mesh.name = src_mesh->mName.data;

        glGenBuffers(1, &dst_mesh.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, dst_mesh.vbo);

        // Positions

        for(unsigned i=0; i< src_mesh->mNumVertices; ++i)
        {
            for(unsigned j=0; j<3; ++j)
            {
                min_v[j] = min(min_v[j], src_mesh->mVertices[i][j]);
                max_v[j] = max(max_v[j], src_mesh->mVertices[i][j]);
            }
        }

        unsigned offset_acc  = sizeof(math::float3);

        if(src_mesh->HasNormals())
        {
            dst_mesh.normals_offset = offset_acc;
            offset_acc += sizeof(math::float3);
        }

        if(src_mesh->HasTextureCoords(0))
        {
            dst_mesh.texcoords_offset = offset_acc;
			offset_acc += sizeof(math::float2);
        }

        dst_mesh.vertex_size = offset_acc;

        glBufferData(GL_ARRAY_BUFFER, dst_mesh.vertex_size*src_mesh->mNumVertices, nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*3*src_mesh->mNumVertices, src_mesh->mVertices);

        // Normals
        
        if(src_mesh->HasNormals())
        {
            glBufferSubData(GL_ARRAY_BUFFER, dst_mesh.normals_offset*src_mesh->mNumVertices, sizeof(float)*3*src_mesh->mNumVertices, src_mesh->mNormals);
        }

        // Texture coords

        if(src_mesh->HasTextureCoords(0))
        {

            math::float2* texture_coords = (math::float2*)glMapBufferRange(GL_ARRAY_BUFFER, dst_mesh.texcoords_offset*src_mesh->mNumVertices, 
                    sizeof(float)*2*src_mesh->mNumVertices, GL_MAP_WRITE_BIT);
            for(unsigned i=0; i< src_mesh->mNumVertices; ++i)
            {
                texture_coords[i] = math::float2(src_mesh->mTextureCoords[0][i].x, src_mesh->mTextureCoords[0][i].y);
            }

            glUnmapBuffer(GL_ARRAY_BUFFER);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Indices

        glGenBuffers(1, &dst_mesh.ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst_mesh.ibo);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*src_mesh->mNumFaces*3, nullptr, GL_STATIC_DRAW);

        unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, 
                                                        sizeof(unsigned)*src_mesh->mNumFaces*3, GL_MAP_WRITE_BIT);

        for(unsigned i=0; i< src_mesh->mNumFaces; ++i)
        {
            assert(src_mesh->mFaces[i].mNumIndices == 3);

            *(indices++) = src_mesh->mFaces[i].mIndices[0];
            *(indices++) = src_mesh->mFaces[i].mIndices[1];
            *(indices++) = src_mesh->mFaces[i].mIndices[2];
        }

        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        dst_mesh.material	  = src_mesh->mMaterialIndex;
        dst_mesh.num_vertices = src_mesh->mNumVertices;
        dst_mesh.num_indices  = src_mesh->mNumFaces*3;

        // generate vao
        
        GenerateVAO(dst_mesh);

        meshes.push_back(dst_mesh);
    }

    bsphere.center = (max_v+min_v)*0.5f;
    bsphere.radius = (max_v-min_v).Length()*0.5f;
}

void ModuleModelLoader::GenerateMaterials(const aiScene* scene)
{
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		const aiMaterial* src_material = scene->mMaterials[i];

        Material dst_material;

        aiString file;
		aiTextureMapping mapping;
		unsigned uvindex = 0;

        if(src_material->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS)
		{
            dst_material.diffuse_map = App->textures->Load(file.data, false);
        }

        dst_material.program = unsigned(ModulePrograms::DEFAULT_PROGRAM);

        materials.push_back(dst_material);
	}
}

