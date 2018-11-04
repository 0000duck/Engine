#pragma once
#include "Module.h"
#include "Globals.h"

#include "ModuleModelLoader.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;
class Camera;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool          Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool          CleanUp();
	void          WindowResized(unsigned width, unsigned height);

private:

    void          RenderMesh(const ModuleModelLoader::Mesh& mesh, const ModuleModelLoader::Material& material, 
                             unsigned program, const math::float4x4& model, 
                             const math::float4x4& view, const math::float4x4& proj);

    void          GenerateFBOTexture(unsigned w, unsigned h);
public:

	void*        context    = nullptr;
    Camera*      camera     = nullptr;
    unsigned     fbo        = 0;
    unsigned     fb_depth   = 0;
    unsigned     fb_tex     = 0;
    unsigned     fb_width   = 0;
    unsigned     fb_height  = 0;

};
