#include "imgui_window.h"

class CImGuiCrosshairs : public IImGuiWindow
{
public:
    static bool m_ShowCrosshairs;
    void Initialize();
    void VidInitialize();
    void Terminate();
    void Think();
    void Draw();
    bool Active();
    bool CursorRequired();
    bool HandleKey(bool keyDown, int keyNumber, const char *bindName);
};

extern CImGuiCrosshairs g_ImGuiCrosshairs;
