#include "Globals.h"
#include "PanelGOTree.h"
#include "Application.h"
#include "modulemodelloader.h"

#include "imgui.h"


PanelGOTree::PanelGOTree()
{
}

PanelGOTree::~PanelGOTree()
{
}

void PanelGOTree::Draw()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 16.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(256.0f, 420.0f), ImGuiCond_FirstUseEver);


    if(ImGui::Begin("GameObjects Hierarchy"))
    {
        for(unsigned i=0; i< App->models->meshes.size(); ++i)
        {
            unsigned flags = ImGuiTreeNodeFlags_Leaf;
            flags |= i == selected ? ImGuiTreeNodeFlags_Selected : 0;

            if(ImGui::TreeNodeEx(App->models->meshes[i].name.c_str(), flags))
            {
                ImGui::TreePop();
            }
        }

        ImGui::End();
    }
}
