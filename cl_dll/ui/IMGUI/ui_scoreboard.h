#include "imgui_window.h"
#include "ui_utils.h"

class CImGuiScoreboard : public IImGuiWindow
{
    CImguiUtils m_ImguiUtils;
public:
    static bool m_ShowWindow;
    void Initialize();
    void VidInitialize();
    void Terminate();
    void Think();
    void DrawScoreboard();
    void Draw();
    bool Active();
    bool CursorRequired();
    bool HandleKey(bool keyDown, int keyNumber, const char *bindName);
};

extern CImGuiScoreboard m_iScoreboard;
