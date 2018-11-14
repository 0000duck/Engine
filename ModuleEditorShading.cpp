#include "Globals.h"

#include "Application.h"

#include "ModuleRender.h"
#include "ModuleModelLoader.h"
#include "ModuleEditorShading.h"
#include "ModulePrograms.h"
#include "ModuleRender.h"

#include "Viewport.h"

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

    LoadShapes(Shapes(shape));
    viewport->CenterCamera();

    ModuleModelLoader* models = App->models;
	models->light.pos = math::float3(3.0f, 3.0f, 3.0f);
    models->ambient   = 0.3f;

    App->render->CenterCamera();

    return ok;
}

update_status ModuleEditorShading::Update()
{
    InitFrame();

    viewport->Draw();

    ImGui::SetNextWindowPos(ImVec2(916.0f, 16.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(256.0f, 420.0f), ImGuiCond_FirstUseEver);

    if(ImGui::Begin("Parameters"))
    {
        ModuleModelLoader::Material& material = App->models->materials[App->models->meshes.back().material];

        char* shape_names[SHAPE_COUNT] = { "Sphere", "Torus", "Cube", "Cylinder" };

        if(ImGui::Combo("shape", (int*)&shape, shape_names, SHAPE_COUNT))
        {
            LoadShapes(Shapes(shape));
        }

        ImGui::ColorEdit4("diffuse color", (float*)&material.diffuse_color);
        ImGui::SliderFloat("shininess", &material.shininess, 0, 128.0f);
        ImGui::SliderFloat("gloss", &material.glossiness, 0.0f, 1.0f);

        ImGui::Separator();

        ImGui::SliderFloat3("light position", (float*)&App->models->light.pos, -10.0f, 10.0f);
        ImGui::SliderFloat("ambient", (float*)&App->models->ambient, 0.0f, 1.0f);

        ImGui::Separator();
        ImGui::Checkbox("show axis", &App->render->show_axis);
        ImGui::Checkbox("auto rotate", &auto_rotate);

        char* show_components[ModuleModelLoader::SHOW_COMPONENT_COUNT] = { "All", "Ambient", "Diffuse", "Specular" };
        ImGui::Combo("show light component", (int*)&material.show_component, show_components, ModuleModelLoader::SHOW_COMPONENT_COUNT);

        for(unsigned i=0; i< App->models->materials.size(); ++i)
        {
            App->models->materials[i].diffuse_color = material.diffuse_color;
            App->models->materials[i].shininess = material.shininess;
            App->models->materials[i].glossiness = material.glossiness;
            App->models->materials[i].show_component = material.show_component;
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

void ModuleEditorShading::LoadShapes(Shapes s)
{
    ModuleModelLoader* models = App->models;
    models->Clear();

    switch(s)
    {
        case CUBE:
            models->LoadCube("cube0", math::float3::zero, math::Quat::identity, 2.0f, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadCube("cube1", math::float3(3.0f, 0.0f, 0.0f), math::Quat::identity, 2.0f, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadCube("cube2", math::float3(6.0f, 0.0f, 0.0f), math::Quat::identity, 2.0f, float4(1.0f, 1.0f, 1.0f, 1.0f));
            break;
        case CYLINDER:
            models->LoadCylinder("cylinder0", math::float3::zero, math::Quat::identity, 2.0f, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadCylinder("cylinder1", math::float3(3.0f, 0.0f, 0.0f), math::Quat::identity, 2.0f, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadCylinder("cylinder2", math::float3(6.0f, 0.0f, 0.0f), math::Quat::identity, 2.0f, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            break;
        case SPHERE:
            models->LoadSphere("sphere0", math::float3::zero, math::Quat::identity, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadSphere("sphere1", math::float3(3.0f, 0.0f, 0.0f), math::Quat::identity, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadSphere("sphere2", math::float3(6.0f, 0.0f, 0.0f), math::Quat::identity, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            break;
        case TORUS:
            models->LoadTorus("torus0", math::float3::zero, math::Quat::identity, 0.5f, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadTorus("torus1", math::float3(3.0f, 0.0f, 0.0f), math::Quat::identity, 0.5f, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            models->LoadTorus("torus2", math::float3(6.0f, 0.0f, 0.0f), math::Quat::identity, 0.5f, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
            break;
    }

    for(unsigned i=0; i< models->materials.size(); ++i)
    {
        ModuleModelLoader::Material& material = models->materials[i];
        material.program       = i+1;
        material.diffuse_color = math::float4(1.0f, 0.0f, 0.0f, 1.0f);
        material.glossiness    = 1.0f;
        material.shininess	   = 32.0f;
    }
}

