#include "Globals.h"
#include "ArcBall.h"
#include "Camera.h"

#include "imgui.h"

#define ROTATION_SPEED 0.025f
#define TRANSLATION_SPEED 0.03f

ArcBall::ArcBall() 
{
}

ArcBall::~ArcBall()
{
}

void ArcBall::Update(Camera* camera)
{
    if(ImGui::IsMouseDragging(0))
    {
        ImVec2 delta = ImGui::GetMouseDragDelta(0);
        float sign_x = delta.x < 0.0f ? -1.0f : 1.0f;
        float sign_y = delta.y < 0.0f ? -1.0f : 1.0f;

        if(ImGui::GetIO().KeyCtrl) // note: must be inside imgui window
        {
            dragging.panning.x= -TRANSLATION_SPEED*delta.x;
            dragging.panning.y= TRANSLATION_SPEED*delta.y;
        }
        else
        {
            dragging.polar= -sign_x*math::pi*(ROTATION_SPEED*delta.x*ROTATION_SPEED*delta.x)/180.0f;
            dragging.azimuthal= sign_y*math::pi*(ROTATION_SPEED*delta.y*ROTATION_SPEED*delta.y)/180.0f;
        }
    }
    else
    {
        params.polar += dragging.polar;
        params.azimuthal += dragging.azimuthal;
        params.panning += dragging.panning;
        dragging.polar = 0.0f;
        dragging.azimuthal = 0.0f;
        dragging.panning = math::float3(0.0f);
    }

    if(ImGui::IsMouseDragging(1))
    {
        ImVec2 delta = ImGui::GetMouseDragDelta(1);
        float sign = delta.y < 0.0f ? -1.0f : 1.0f;
        dragging.radius = sign*(TRANSLATION_SPEED*delta.y)*(TRANSLATION_SPEED*delta.y);
        if (dragging.radius < -params.radius)
        {
            dragging.radius = -params.radius;
        }
    }
    else
    {
        params.radius += dragging.radius;
        dragging.radius = 0.0f;
    }

    if(camera)
    {
        UpdateCamera(camera);
    }
}

void ArcBall::UpdateCamera(Camera* camera)
{
	assert(camera);

    math::Quat rotation_polar(math::float3(0.0f, 1.0f, 0.0f), dragging.polar+params.polar);
    math::Quat rotation_azimuthal(math::float3(1.0f, 0.0f, 0.0f), dragging.azimuthal+params.azimuthal);
    math::Quat rotation = rotation_polar*rotation_azimuthal;
	camera->SetPosition(rotation.Transform(dragging.panning+params.panning+math::float3(0.0f, 0.0f, dragging.radius+params.radius)));
	camera->SetRotation(rotation);
}

void ArcBall::SetRadius(float radius)
{
	params.radius = radius;
}

void ArcBall::SetPolar(float polar)
{
	params.polar = polar;
}

void ArcBall::SetAzimuthal(float azimuthal)
{
	params.azimuthal = azimuthal;
}

void ArcBall::SetPanning(const math::float3& panning)
{
    params.panning = panning;
}
