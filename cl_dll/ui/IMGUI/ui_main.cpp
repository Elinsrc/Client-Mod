#include "imgui_window_system.h"
#include "ui_demo_window.h"
#include "ui_scoreboard.h"
#include "ui_commands.h"

void CImGuiWindowSystem::LinkWindows()
{
    static CImGuiDemoWindow demoWindow;
    AddWindow(&demoWindow);

    static CImGuiScoreboard scoreWindow;
    AddWindow(&scoreWindow);

    static CImGuiCommands commandsWindow;
    AddWindow(&commandsWindow);
}
