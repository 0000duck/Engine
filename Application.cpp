#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModulePrograms.h"

#include "ModuleModelLoader.h"

#include "ModuleEditorScene.h"
#include "ModuleEditorShading.h"

#include "ModuleDebugDraw.h"

#include "ModulePhysics.h"


using namespace std;

Application::Application(int argc, char** argv)
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(render= new ModuleRender());
	modules.push_back(debug_draw = new ModuleDebugDraw());
	modules.push_back(textures = new ModuleTextures());
    modules.push_back(programs = new ModulePrograms());
	modules.push_back(input = new ModuleInput());
    modules.push_back(models = new ModuleModelLoader());

    if(argc > 1 && _stricmp(argv[1], "shading") == 0)
    {
        modules.push_back(editor = new ModuleEditorShading());
    }
    else if(argc > 1 && _stricmp(argv[1], "physics") == 0)
    {
        modules.push_back(editor = new ModulePhysics());
    }
    else 
    {
        modules.push_back(editor = new ModuleEditorScene());
    }
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}
