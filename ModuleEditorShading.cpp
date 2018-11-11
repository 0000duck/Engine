#include "Globals.h"

#include "Application.h"

#include "ModuleRender.h"
#include "ModuleModelLoader.h"
#include "ModuleEditorShading.h"
#include "ModulePrograms.h"

#include "Viewport.h"

#include "DebugDraw.h"

ModuleEditorShading::ModuleEditorShading()
{
}

ModuleEditorShading::~ModuleEditorShading()
{
}

bool ModuleEditorShading::Init()
{
    bool ok = ModuleEditor::Init();

    App->models->LoadSphere("sphere0", 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));

    ModuleModelLoader* models = App->models;
    ModuleModelLoader::Material& material = models->materials[App->models->meshes.back().material];
    material.program       = unsigned(ModulePrograms::GOURAUD_PROGRAM);
    material.diffuse_color = math::float4(1.0f, 0.0f, 0.0f, 1.0f);
	material.glossiness    = 1.0f;
	material.shininess	   = 32.0f;
    models->ambient        = 0.1f;
	models->light.pos	   = math::float3(2.0f, 2.0f, 2.0f);


    App->render->CenterCamera();

    viewport = new Viewport;

    return ok;
}

update_status ModuleEditorShading::Update()
{
    InitFrame();

    viewport->Draw();

    EndFrame();

    return UPDATE_CONTINUE;
}

bool ModuleEditorShading::CleanUp()
{
    return ModuleEditor::CleanUp();
}
