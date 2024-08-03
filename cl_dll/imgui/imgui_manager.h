#pragma once
#include "imgui_window_system.h"
#include <map>

class CImGuiManager
{
public:
    static CImGuiManager &GetInstance();

    void Initialize();
    void VidInitialize();
    void Terminate();
    void NewFrame();
    bool KeyInput(bool keyDown, int keyNumber, const char *bindName);
    static void TextInputCallback(const char *text);

private:
    struct MouseButtonsState
    {
        bool left = false;
        bool middle = false;
        bool right = false;
    };

    void LoadFonts();
    void ApplyStyles();
    void UpdateMouseState();
    void UpdateCursorState();
    void HandleKeyInput(bool keyDown, int keyNumber);
    bool HandleMouseInput(bool keyDown, int keyNumber);
    void SetupConfig();
    void SetupKeyboardMapping();

    bool m_bWasCursorRequired = false;
    MouseButtonsState m_MouseButtonsState;
    std::map<int, int> m_KeysMapping;
    CImGuiWindowSystem m_WindowSystem;
};
extern CImGuiManager &g_ImGuiManager;
