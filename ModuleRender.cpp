#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleModelLoader.h"
#include "ModulePrograms.h"
#include "Camera.h"

#include "SDL.h"
#include "GL/glew.h"

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	context = SDL_GL_CreateContext(App->window->window);

	glewInit();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_2D);

	glClearDepth(1.0f);
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);

    int width, height;
    SDL_GetWindowSize(App->window->window, &width, &height);
    glViewport(0, 0, width, height);

    camera = new Camera;
    camera->LookAt(math::float3(0.0f, 5.0f, 10.0f), math::float3(0.0f, 5.0f, 0.0f), math::float3::unitY);
    camera->SetPerspective(math::pi*0.25f, float(width)/float(height), 0.1f, 150.0f);

	return true;
}

update_status ModuleRender::PreUpdate()
{
    int width, height;
    SDL_GetWindowSize(App->window->window, &width, &height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
    math::float4x4 proj = camera->GetProjMatrix();
    math::float4x4 view = camera->GetViewMatrix();

    for(unsigned i=0; i< App->models->meshes.size(); ++i)
    {
        const ModuleModelLoader::Mesh& mesh = App->models->meshes[i];

        RenderMesh(mesh, App->models->materials[mesh.material], App->programs->def_program, 
				  App->models->transform, view, proj);
    }

	return UPDATE_CONTINUE;
}

void ModuleRender::RenderMesh(const ModuleModelLoader::Mesh& mesh, const ModuleModelLoader::Material& material, 
                              unsigned program, const math::float4x4& model, 
                              const math::float4x4& view, const math::float4x4& proj)
{
    glUseProgram(program);

    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&model);
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
    glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.texture0);
    glUniform1i(glGetUniformLocation(program, "texture0"), 0); 

    glBindVertexArray(mesh.vao);
	glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);

}

update_status ModuleRender::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

    delete camera;
    camera = nullptr;

    SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
    glViewport(0, 0, width, height); 
}

