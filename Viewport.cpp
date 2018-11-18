#include "Globals.h"
#include "Viewport.h"

#include "Application.h"

#include "ModuleRender.h"

#include "Camera.h"
#include "ArcBall.h"

#include "imgui.h"
#include "GL/glew.h"

Viewport::Viewport()
{
    camera = new Camera;

    camera_ctrl = new ArcBall;
    CenterCamera();
}

Viewport::~Viewport()
{
    delete camera_ctrl;
    delete camera;
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
                camera_ctrl->Update(camera);
            }
            else
            {
                camera_ctrl->UpdateCamera(camera);
            }

            float width  = ImGui::GetWindowContentRegionWidth();
            float height = ImGui::GetContentRegionAvail().y;

            camera->SetPerspective(math::pi*0.25f, float(width)/float(height), 0.1f, 150.0f);
            GenerateFBOTexture(unsigned(width), unsigned(height));

            App->render->Draw(camera, fbo, fb_width, fb_height);

            ImGui::GetWindowDrawList()->AddImage(
                    (void*)fb_tex,
                    ImVec2(ImGui::GetCursorScreenPos()),
                    ImVec2(ImGui::GetCursorScreenPos().x + fb_width,
                           ImGui::GetCursorScreenPos().y + fb_height), 
                           ImVec2(0, 1), ImVec2(1, 0));

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
        ImGui::EndChild();
    }
	ImGui::End();
}

void Viewport::CenterCamera()
{
    camera_ctrl->SetPanning(App->models->bsphere.center);
    camera_ctrl->SetRadius(max(App->models->bsphere.radius*1.5f, 2.0f));
}

void Viewport::GenerateFBOTexture(unsigned w, unsigned h)
{
    if(w != fb_width || h != fb_height)
    {
        if(fb_tex != 0)
        {
            glDeleteTextures(1, &fb_tex);
        }

        if(w != 0 && h != 0)
        {
            if(fbo == 0)
            {
                glGenFramebuffers(1, &fbo);
            }

            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glGenTextures(1, &fb_tex);
            glBindTexture(GL_TEXTURE_2D, fb_tex);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
            glGenRenderbuffers(1, &fb_depth);
            glBindRenderbuffer(GL_RENDERBUFFER, fb_depth);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fb_depth);            
            glBindRenderbuffer(GL_RENDERBUFFER, 0);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_tex, 0);

            glDrawBuffer(GL_COLOR_ATTACHMENT0);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

		fb_width = w;
		fb_height = h;
    }
}

