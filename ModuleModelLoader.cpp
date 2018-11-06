#include "Globals.h"

#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleTextures.h"

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
	const aiScene* scene = aiImportFile("suzanne.fbx", aiProcessPreset_TargetRealtime_MaxQuality);

    if(scene)
    {
        GenerateMeshes(scene);
        GenerateMaterials(scene);
		
		aiReleaseImport(scene);

        return true;
    }

	LOG("Error loading BakerHouse.fbx: %s", aiGetErrorString());

    return false;
}

update_status ModuleModelLoader::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleModelLoader::CleanUp()
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
		if (materials[i].texture0 != 0)
		{
			App->textures->Unload(materials[i].texture0);
		}
	}
	return true;
}

void ModuleModelLoader::GenerateMeshes(const aiScene* scene)
{
    math::float3 min_v(FLT_MAX, FLT_MAX, FLT_MAX);
    math::float3 max_v(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for(unsigned i=0; i< scene->mNumMeshes; ++i)
    {
        const aiMesh* src_mesh = scene->mMeshes[i];

        Mesh dst_mesh;

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

        dst_mesh.vertex_size |= src_mesh->HasTextureCoords(0) ? sizeof(float3)+sizeof(float2) : sizeof(float3);

        glBufferData(GL_ARRAY_BUFFER, dst_mesh.vertex_size*src_mesh->mNumVertices, nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*3*src_mesh->mNumVertices, src_mesh->mVertices);

        // Texture coords

        if(src_mesh->HasTextureCoords(0))
        {
            dst_mesh.attribs |= ATTRIB_TEX_COORDS_0;

            math::float2* texture_coords = (math::float2*)glMapBufferRange(GL_ARRAY_BUFFER, sizeof(float)*3*src_mesh->mNumVertices, 
                    sizeof(float)*2*src_mesh->mNumVertices, GL_MAP_WRITE_BIT);
            for(unsigned i=0; i< src_mesh->mNumVertices; ++i)
            {
                texture_coords[i] = math::float2(src_mesh->mTextureCoords[0][i].x, src_mesh->mTextureCoords[0][i].y);
            }
        }

        glUnmapBuffer(GL_ARRAY_BUFFER);
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
        glGenVertexArrays(1, &dst_mesh.vao);

        glBindVertexArray(dst_mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, dst_mesh.vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst_mesh.ibo);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, dst_mesh.vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float)*3*dst_mesh.num_vertices));

        glBindVertexArray(0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

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
            dst_material.texture0 = App->textures->Load(file.data, false);
        }

        materials.push_back(dst_material);
	}
}

