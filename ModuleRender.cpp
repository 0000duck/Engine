#include "Globals.h"
#include "ModuleRender.h"

#include "Application.h"

#include "ModuleWindow.h"
#include "ModuleModelLoader.h"
#include "ModulePrograms.h"
#include "ModuleDebugDraw.h"

#include "Camera.h"
#include "debugdraw.h"

#include "SDL.h"
#include "GL/glew.h"

void __stdcall DebugMessageGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (id == 131185 || id == 131204) return;

	char tmp_string[4096];

	const char* tmp_source = "";

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             tmp_source = "API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   tmp_source = "Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: tmp_source = "Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     tmp_source = "Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     tmp_source = "Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           tmp_source = "Other"; break;
	};

	const char* tmp_type = "";

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               tmp_type = "Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: tmp_type = "Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  tmp_type = "Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         tmp_type = "Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         tmp_type = "Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              tmp_type = "Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          tmp_type = "Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           tmp_type = "Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               tmp_type = "Other"; break;
	};


	const char* tmp_severity = "";
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         tmp_severity = "high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       tmp_severity = "medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          tmp_severity = "low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: tmp_severity = "notification"; break;
	};

	sprintf_s(tmp_string, 4095, "<Source:%s> <Type:%s> <Severity:%s> <ID:%d> <Message:%s>\n", tmp_source, tmp_type, tmp_severity, id, message);
	OutputDebugString(tmp_string);
}


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

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

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

#if _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

	glDebugMessageCallback(&DebugMessageGL, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);

	glClearDepth(1.0f);
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);

	return true;
}

update_status ModuleRender::PreUpdate()
{
    int width, height;
    SDL_GetWindowSize(App->window->window, &width, &height);
    glViewport(0, 0, width, height);
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	return UPDATE_CONTINUE;
}

void ModuleRender::Draw(Camera* camera, unsigned fbo, unsigned fb_width, unsigned fb_height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glViewport(0, 0, fb_width, fb_height);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    math::float4x4 proj = camera->GetProjMatrix();
    math::float4x4 view = camera->GetViewMatrix();

    App->debug_draw->Draw(camera, fbo, fb_height, fb_height);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    for(unsigned i=0; i< App->models->meshes.size(); ++i)
    {
        const ModuleModelLoader::Mesh& mesh = App->models->meshes[i];

        RenderMesh(mesh, App->models->materials[mesh.material], mesh.transform, view, proj);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void ModuleRender::RenderMesh(const ModuleModelLoader::Mesh& mesh, const ModuleModelLoader::Material& material,
	                          const math::float4x4& model, const math::float4x4& view, const math::float4x4& proj)
{
	unsigned program = App->programs->programs[material.program];
	glUseProgram(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&model);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);

    glUniform3fv(glGetUniformLocation(program, "light_pos"), 1, (const float*)&App->models->light.pos);
    glUniform1f(glGetUniformLocation(program,  "ambient"), App->models->ambient);
    glUniform1f(glGetUniformLocation(program, "shininess"), material.shininess);
    glUniform1f(glGetUniformLocation(program, "k_ambient"), material.k_ambient);
    glUniform1f(glGetUniformLocation(program, "k_diffuse"), material.k_diffuse);
    glUniform1f(glGetUniformLocation(program, "k_specular"), material.k_specular);

    if (material.diffuse_map == 0)
    {
        glUniform1i(glGetUniformLocation(program, "use_diffuse_map"), 0);
        glUniform4fv(glGetUniformLocation(program, "object_color"), 1, (const float*)&material.object_color);
    }
    else
    {
        glUniform1i(glGetUniformLocation(program, "use_diffuse_map"), 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material.diffuse_map);
        glUniform1i(glGetUniformLocation(program, "diffuse_map"), 0);
    }

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

    SDL_GL_DeleteContext(context);

	return true;
}



