#include "Globals.h"

#include "Application.h"

#include "ModuleRender.h"
#include "ModuleModelLoader.h"
#include "ModuleEditorShading.h"
#include "ModulePrograms.h"
#include "ModuleRender.h"

#include "Viewport.h"
#include "Camera.h"
#include "DebugDraw.h"

#include "imgui.h"

ModuleEditorShading::ModuleEditorShading()
{
}

ModuleEditorShading::~ModuleEditorShading()
{
}

bool ModuleEditorShading::Init()
{
    bool ok = ModuleEditor::Init();

    viewport = new Viewport;

    ModuleModelLoader::Material material;
    material.object_color = math::float4(0.6f, 0.0f, 0.0f, 1.0f);
    material.k_specular   = 0.6f;
    material.k_diffuse    = 0.3f;
    material.k_ambient    = 1.0f;
    material.shininess	  = 32.0f;

    LoadShapes(Shapes(shape), material);
    viewport->CenterCamera();

    ModuleModelLoader* models = App->models;
	models->light.pos = math::float3(6.0f, 8.0f, 3.0f);
    models->ambient   = 0.2f;

    return ok;
}

update_status ModuleEditorShading::Update()
{
    InitFrame();

    DrawDebugData();

    viewport->Draw();

    ImGui::SetNextWindowPos(ImVec2(916.0f, 16.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(256.0f, 420.0f), ImGuiCond_FirstUseEver);

    if(ImGui::Begin("Parameters"))
    {
        ModuleModelLoader::Material& material = App->models->materials[App->models->meshes.back().material];

        char* shape_names[SHAPE_COUNT] = { "Sphere", "Torus", "Cube", "Cylinder" };

        if(ImGui::CollapsingHeader("Shape and material"))
        {
            for(unsigned i=0; i< SHAPE_COUNT; ++i)
            {
                if(ImGui::RadioButton(shape_names[i], shape == Shapes(i)))
                {
                    ModuleModelLoader::Material material = App->models->materials[0];
                    LoadShapes(Shapes(i), material);
                    shape = i;
                }
            }

            ImGui::ColorEdit4("object color", (float*)&material.object_color);
            ImGui::SliderFloat("shininess", &material.shininess, 0, 128.0f);
            ImGui::SliderFloat("K ambient", &material.k_ambient, 0.0f, 1.0f);
            ImGui::SliderFloat("K diffuse", &material.k_diffuse, 0.0f, 1.0f);
            ImGui::SliderFloat("K specular", &material.k_specular, 0.0f, 1.0f);
        }

        if(ImGui::CollapsingHeader("Light"))
        {
            ImGui::SliderFloat3("light position", (float*)&App->models->light.pos, -15.0f, 15.0f);
            ImGui::SliderFloat("ambient", (float*)&App->models->ambient, 0.0f, 1.0f);
        }

        if(ImGui::CollapsingHeader("Options"))
        {
            ImGui::Checkbox("show axis", &show_axis);
            ImGui::Checkbox("show grid", &show_grid);
            ImGui::Checkbox("auto rotate", &auto_rotate);
        }

        for(unsigned i=0; i< App->models->materials.size(); ++i)
        {
            App->models->materials[i].object_color = material.object_color;
            App->models->materials[i].shininess = material.shininess;
            App->models->materials[i].k_ambient = material.k_ambient;
            App->models->materials[i].k_diffuse = material.k_diffuse;
            App->models->materials[i].k_specular = material.k_specular;
        }

        ImGui::End();
    }

    EndFrame();

    if(auto_rotate)
    {
        for(unsigned i=0; i< App->models->meshes.size(); ++i)
        {
            ModuleModelLoader::Mesh& mesh  = App->models->meshes[i];
            math::Quat r;
            math::float3 t, s;
            mesh.transform.Decompose(t, r, s);

            math::Quat q(math::float3(0.0f, 1.0f, 0.0f), 0.001f);

            mesh.transform = math::float4x4(q*r, t);
        }
    }

    return UPDATE_CONTINUE;
}

bool ModuleEditorShading::CleanUp()
{
    return ModuleEditor::CleanUp();
}

void ModuleEditorShading::LoadShapes(Shapes s, const ModuleModelLoader::Material& material)
{
    ModuleModelLoader* models = App->models;
    models->Clear();

    switch(s)
    {
        case CUBE:
            models->LoadCube("cube0", math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 2.0f, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadCube("cube1", math::float3(5.0f, 2.0f, 0.0f), math::Quat::identity, 2.0f, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadCube("cube2", math::float3(8.0f, 2.0f, 0.0f), math::Quat::identity, 2.0f, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadCube("cube3", math::float3(11.0f, 2.0f, 0.0f), math::Quat::identity, 2.0f, float4(1.0f, 1.0f, 1.0f, 1.0f));
            break;
        case CYLINDER:
            models->LoadCylinder("cylinder0", math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 2.0f, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadCylinder("cylinder1", math::float3(5.0f, 2.0f, 0.0f), math::Quat::identity, 2.0f, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadCylinder("cylinder2", math::float3(8.0f, 2.0f, 0.0f), math::Quat::identity, 2.0f, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadCylinder("cylinder3", math::float3(11.0f, 2.0f, 0.0f), math::Quat::identity, 2.0f, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            break;
        case SPHERE:
            models->LoadSphere("sphere0", math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadSphere("sphere1", math::float3(5.0f, 2.0f, 0.0f), math::Quat::identity, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadSphere("sphere2", math::float3(8.0f, 2.0f, 0.0f), math::Quat::identity, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadSphere("sphere3", math::float3(11.0f, 2.0f, 0.0f), math::Quat::identity, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            break;
        case TORUS:
            models->LoadTorus("torus0", math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 0.5f, 0.67f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadTorus("torus1", math::float3(5.0f, 2.0f, 0.0f), math::Quat::identity, 0.5f, 0.67f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadTorus("torus2", math::float3(8.0f, 2.0f, 0.0f), math::Quat::identity, 0.5f, 0.67f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadTorus("torus3", math::float3(11.0f, 2.0f, 0.0f), math::Quat::identity, 0.5f, 0.67f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            break;
    }

    for(unsigned i=0; i< models->materials.size(); ++i)
    {
        models->materials[i] = material;
        models->materials[i].program = i+1;
    }
}

