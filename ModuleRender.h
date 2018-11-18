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

	bool          Init              ();
	update_status PreUpdate         ();
	update_status Update            ();
	update_status PostUpdate        ();
	bool          CleanUp           ();

    void          CenterCamera      ();
    void          Draw              (Camera* camera, unsigned fbo, unsigned fb_width, unsigned fb_height);

private:

    void          RenderMesh        (const ModuleModelLoader::Mesh& mesh, const ModuleModelLoader::Material& material, 
                                    const math::float4x4& model, const math::float4x4& view, const math::float4x4& proj);

public:

	void*        context    = nullptr;

};
