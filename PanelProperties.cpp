#include "Globals.h"

#include "PanelProperties.h"

#include "Application.h"

#include "ModuleEditor.h"
#include "ModuleModelLoader.h"
#include "ModulePrograms.h"

#include "PanelGOTree.h"

#include "GL/glew.h"

#include "imgui.h"

PanelProperties::PanelProperties()
{
}

PanelProperties::~PanelProperties()
{
}

void PanelProperties::Draw(unsigned selected)
{
    ImGui::SetNextWindowPos(ImVec2(916.0f, 16.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(256.0f, 420.0f), ImGuiCond_FirstUseEver);

    if(ImGui::Begin("Properties"))
    {
        if(selected < App->models->meshes.size())
        {
            ModuleModelLoader::Mesh& mesh = App->models->meshes[selected];

            // Text Input for the name
            char name[50];
            strcpy_s(name, 50, mesh.name.c_str());
            if (ImGui::InputText("name", name, 50,  ImGuiInputTextFlags_EnterReturnsTrue))
            {
                mesh.name = name;
            }

            // \todo: transform

            DrawMesh(selected);
            DrawMaterial(mesh.material);
        }

        ImGui::End();
    }
}

void PanelProperties::DrawMesh(unsigned index)
{
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ModuleModelLoader::Mesh& mesh = App->models->meshes[index];
        ImGui::Text("%d vertices", mesh.num_vertices);
        ImGui::Text("%d triangles", mesh.num_indices/3);
    }
}

void PanelProperties::DrawMaterial(unsigned index)
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ModuleModelLoader::Material& material = App->models->materials[index];
        char* program_names[ModulePrograms::PROGRAM_COUNT] = { "Default", "Flat", "Gouraud", "Phong", "Blinn" };
        ImGui::Combo("shader", (int*)&material.program, program_names, ModulePrograms::PROGRAM_COUNT);

        if(material.diffuse_map)
        {
            ImGui::Image((ImTextureID)material.diffuse_map, ImVec2(128.0f, 128.0f));
        }

        ImGui::ColorEdit4("object color", (float*)&material.object_color);

        ImGui::SliderFloat("shininess", &material.shininess, 0, 128.0f);
        ImGui::SliderFloat("k ambient", &material.k_ambient, 0.0f, 1.0f);
        ImGui::SliderFloat("k diffsue", &material.k_diffuse, 0.0f, 1.0f);
        ImGui::SliderFloat("k specular", &material.k_specular, 0.0f, 1.0f);
    }
}

