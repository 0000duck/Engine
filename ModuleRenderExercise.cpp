#include "Application.h"

#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"
#include "ModulePrograms.h"
#include "ModuleTextures.h"

#include "GL/glew.h"
#include "SDL.h"

#include "Geometry/Frustum.h"

ModuleRenderExercise::ModuleRenderExercise()
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}

bool ModuleRenderExercise::Init()
{
	float vertex_buffer_data[] =
	{
        // positions
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,

        // uvs
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

    int width, height;
    SDL_GetWindowSize(App->window->window, &width, &height);
    glViewport(0, 0, width, height);

	Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos   = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up    = float3::unitY;

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov   = math::pi/4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * float(width)/float(height));

	proj = frustum.ProjectionMatrix();

	LookAt(view, math::float3(0.0f, 0.0f, 4.0f), math::float3(0.0f, 0.0f, 0.0f), math::float3(0.0f, 1.0f, 0.0f));

	tri_model = math::float4x4::identity;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    texture0 = App->textures->Load("Lenna.png", false);

    return vbo;
}

update_status ModuleRenderExercise::Update()
{
    glUseProgram(App->programs->def_program);

    glUniformMatrix4fv(glGetUniformLocation(App->programs->def_program, "model"), 1, GL_TRUE, &tri_model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(App->programs->def_program, "view"), 1, GL_TRUE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(App->programs->def_program, "proj"), 1, GL_TRUE, &proj[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glUniform1i(glGetUniformLocation(App->programs->def_program, "texture0"), 0); // 0 is related to GL_TEXTURE0

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(
            0,                  // attribute 0
            3,                  // number of componentes (3 floats)
            GL_FLOAT,           // data type
            GL_FALSE,           // should be normalized?
            0,                  // stride
            (void*)0            // array buffer offset
            );
    glVertexAttribPointer(
            1,                          
            2,                          
            GL_FLOAT,                   
            GL_FALSE,                   
            0,                          
            (void*)(sizeof(float)*3*6)    // 3 float 6 vertices for jump positions
            );

    glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);

	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
    if(vbo != 0)
    {
        glDeleteBuffers(1, &vbo);
    }

    if(texture0 != 0)
    {
        App->textures->Unload(texture0);
    }

	return true;
}

void ModuleRenderExercise::LookAt(math::float4x4&  matrix, const math::float3& eye, const math::float3& target, const math::float3& up)
{
    math::float3 f(target - eye);
	f.Normalize();

    math::float3 s(f.Cross(up));
	s.Normalize();

    math::float3 u(s.Cross(f));

	// axis in rows to do inverse
    matrix[0][0] = s.x;
    matrix[0][1] = s.y;
    matrix[0][2] = s.z;

    matrix[1][0] = u.x;
    matrix[1][1] = u.y;
    matrix[1][2] = u.z;

    matrix[2][0] =-f.x;
    matrix[2][1] =-f.y;
    matrix[2][2] =-f.z;

	// mult inverted rotation matrix negate to make inverse
    matrix[0][3] =-s.Dot(eye);
    matrix[1][3] =-u.Dot(eye);
    matrix[2][3] = f.Dot(eye);

	matrix[3][0] = 0.0f;
	matrix[3][1] = 0.0f;
	matrix[3][2] = 0.0f;
	matrix[3][3] = 1.0f;
}

