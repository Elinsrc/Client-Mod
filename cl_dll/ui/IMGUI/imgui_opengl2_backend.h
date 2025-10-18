#pragma once
#include "imgui.h"

class CImGuiOpenGL2Backend
{

public:
    bool Init();
    void Shutdown();
    void NewFrame();
    void RenderDrawData(ImDrawData* draw_data);
};

static void ImGui_ImplOpenGL2_UpdateTexture(ImTextureData* tex);
static bool ImGui_ImplOpenGL2_CreateDeviceObjects();
static void ImGui_ImplOpenGL2_DestroyDeviceObjects();
