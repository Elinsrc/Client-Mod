#include "imgui.h"
#include "imgui_int.h"
#include "imgui_impl_opengl3.h"
#include "hud.h"
#include "cl_util.h"

namespace hl_imgui
{
    void Initialize()
    {
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.IniFilename = NULL;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.Fonts->AddFontDefault();
        ImGui::StyleColorsDark();
        ImGui_ImplOpenGL3_Init(nullptr);
    }

    void Terminate()
    {
        ImGui_ImplOpenGL3_Shutdown();
    }

    void NewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        bool show_demo_window = true;
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }


}
