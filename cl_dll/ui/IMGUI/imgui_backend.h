#pragma once
#include "hud.h"
#include "cl_util.h"
#include "imgui.h"

class CImGuiBackend
{
    cvar_t *ui_imgui_scale;

public:
    bool Init();
    void Shutdown();
    void NewFrame();
    void RenderDrawData(ImDrawData *draw_data);
};

bool ImGui_ImplOpenGL2_CreateDeviceObjects();
void ImGui_ImplOpenGL2_DestroyDeviceObjects();
void ImGui_ImplOpenGL2_UpdateTexture(ImTextureData* tex);
