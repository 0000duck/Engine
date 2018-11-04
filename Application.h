#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleEditor;
class ModuleTextures;
class ModuleInput;
class ModulePrograms;
class ModuleRenderExercise;
class ModuleModelLoader;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* render = nullptr;
	ModuleWindow* window = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleInput* input = nullptr;
    ModulePrograms* programs = nullptr;
    ModuleModelLoader* models = nullptr;

private:

	std::list<Module*> modules;

};

extern Application* App;
