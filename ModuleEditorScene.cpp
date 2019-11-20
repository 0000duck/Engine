#include "Globals.h"

#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleEditorScene.h"

#include "Viewport.h"
#include "PanelGOTree.h"
#include "PanelProperties.h"

#include "DebugDraw.h"

ModuleEditorScene::ModuleEditorScene()
{
}

ModuleEditorScene::~ModuleEditorScene()
{
}

bool ModuleEditorScene::Init()
{
    bool ok = ModuleEditor::Init();

    App->models->Load("BakerHouse.fbx");

    if(ok)
    {
        viewport    = new Viewport;
        go_tree     = new PanelGOTree;
        properties  = new PanelProperties;
    }

    return ok;
}

update_status ModuleEditorScene::Update()
{
    InitFrame();

    DrawDebugData();
	dd::sphere(App->models->light.pos, math::float3(1.0f, 1.0f, 1.0f), 0.05f);
    viewport->Draw();
    //go_tree->Draw();
    //properties->Draw(go_tree->selected);

    EndFrame();

    return UPDATE_CONTINUE;
}

bool ModuleEditorScene::CleanUp()
{
    delete go_tree;
    delete properties;
    delete viewport;

    return ModuleEditor::CleanUp();
}
