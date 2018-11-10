#include "Globals.h"
#include "ModuleEditor.h"

#include "ModuleWindow.h"
#include "ModuleRender.h"

#include "Application.h"

#include "PanelGOTree.h"

#include "ArcBall.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

ModuleEditor::ModuleEditor()
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->render->context);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    camera_ctrl = new ArcBall;
    camera_ctrl->SetPanning(App->models->bsphere.center);
    camera_ctrl->SetRadius(App->models->bsphere.radius*2.0f);

    go_tree = new PanelGOTree;

    return true;
}

update_status ModuleEditor::PreUpdate()
{

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(256.0f, 16.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(640.0f, 420.0f), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Scene"))
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

            float width = ImGui::GetWindowContentRegionWidth();
            float height = ImGui::GetContentRegionAvail().y;

            App->render->WindowResized((unsigned)width, (unsigned)height);

            ImGui::GetWindowDrawList()->AddImage(
                    (void*)App->render->fb_tex,
                    ImVec2(ImGui::GetCursorScreenPos()),
                    ImVec2(ImGui::GetCursorScreenPos().x + App->render->fb_width,
                        ImGui::GetCursorScreenPos().y + App->render->fb_height), ImVec2(0, 1), ImVec2(1, 0));

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        }
        ImGui::EndChild();
    }
	ImGui::End();

    go_tree->Draw();

    ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_MakeCurrent(App->window->window, App->render->context);

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    delete go_tree;

	return true;
}

