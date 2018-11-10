#include "Globals.h"
#include "Viewport.h"

#include "Application.h"

#include "ModuleRender.h"

#include "ArcBall.h"

#include "imgui.h"

Viewport::Viewport()
{
    camera_ctrl = new ArcBall;
    camera_ctrl->SetPanning(App->models->bsphere.center);
    camera_ctrl->SetRadius(App->models->bsphere.radius*2.0f);
}

Viewport::~Viewport()
{
    delete camera_ctrl;
}

void Viewport::Draw()
{
    ImGui::SetNextWindowPos(ImVec2(256.0f, 0.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(640.0f, 420.0f), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Viewport"))
	{
        if(ImGui::BeginChild("SceneCanvas", ImVec2(0, 0), true, ImGuiWindowFlags_NoMove))
        {
            if(ImGui::IsWindowHovered())
            {
                camera_ctrl->Update(App->render->camera);
            }
            else
            {
                camera_ctrl->UpdateCamera(App->render->camera);
            }

            float width  = ImGui::GetWindowContentRegionWidth();
            float height = ImGui::GetContentRegionAvail().y;

            App->render->WindowResized((unsigned)width, (unsigned)height);

            ImGui::GetWindowDrawList()->AddImage(
                    (void*)App->render->fb_tex,
                    ImVec2(ImGui::GetCursorScreenPos()),
                    ImVec2(ImGui::GetCursorScreenPos().x + App->render->fb_width,
                           ImGui::GetCursorScreenPos().y + App->render->fb_height), 
                           ImVec2(0, 1), ImVec2(1, 0));

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
        ImGui::EndChild();
    }
	ImGui::End();
}
