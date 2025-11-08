#include "ui_utils.h"
#include "hud.h"
#include <string>
#include <cmath>

ImVec4 CImguiUtils::ColorFromCode(char code)
{
    switch(code)
    {
        case '0': return ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // Black
        case '1': return ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
        case '2': return ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Green
        case '3': return ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
        case '4': return ImVec4(0.0f, 0.0f, 1.0f, 1.0f); // Blue
        case '5': return ImVec4(0.0f, 1.0f, 1.0f, 1.0f); // Cyan
        case '6': return ImVec4(1.0f, 0.0f, 1.0f, 1.0f); // Magenta
        case '7': return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White
        case '8': return ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // Black (same as 0)
        case '9': return ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red (same as 1)
    }
}

void CImguiUtils::TextWithColorCodes(const char* text)
{
    const char* ptr = text;
    ImVec4 currentColor = ImVec4(1.0f, 0.65f, 0.0f, 1.0f); // default orange

    while (*ptr)
    {
        if (*ptr == '^' && *(ptr+1))
        {
            currentColor = ColorFromCode(*(ptr+1));
            ptr += 2;
            continue;
        }

        const char* start = ptr;
        while (*ptr && !(*ptr == '^' && *(ptr+1))) ptr++;
        std::string segment(start, ptr);

        if (!segment.empty())
        {
            ImGui::TextColored(currentColor, "%s", segment.c_str());
            ImGui::SameLine(0.0f, 0.0f);
        }
    }
    ImGui::NewLine();
}

void CImguiUtils::HUEtoRGB(float hue, RGBColor &color)
{
    hue = fmax(0, fmin(255, hue));
    float h = hue / 255.0f;
    float r, g, b;

    if (h < 1.0f/6.0f) {
        r = 1.0f;
        g = h * 6.0f;
        b = 0.0f;
    }
    else if (h < 2.0f/6.0f) {
        r = 1.0f - (h - 1.0f/6.0f) * 6.0f;
        g = 1.0f;
        b = 0.0f;
    }
    else if (h < 3.0f/6.0f) {
        r = 0.0f;
        g = 1.0f;
        b = (h - 2.0f/6.0f) * 6.0f;
    }
    else if (h < 4.0f/6.0f) {
        r = 0.0f;
        g = 1.0f - (h - 3.0f/6.0f) * 6.0f;
        b = 1.0f;
    }
    else if (h < 5.0f/6.0f) {
        r = (h - 4.0f/6.0f) * 6.0f;
        g = 0.0f;
        b = 1.0f;
    }
    else {
        r = 1.0f;
        g = 0.0f;
        b = 1.0f - (h - 5.0f/6.0f) * 6.0f;
    }

    color.r = static_cast<int>(r * 255);
    color.g = static_cast<int>(g * 255);
    color.b = static_cast<int>(b * 255);
}

void CImguiUtils::DrawModelName(float topcolor, float bottomcolor, const char* model)
{
    if (!model)
        return;

    size_t modelLength = strlen(model);
    size_t mid = modelLength / 2;

    std::string firstcolor(model, mid);
    std::string secondcolor(model + mid, modelLength - mid);

    RGBColor top, bottom;
    HUEtoRGB(topcolor, top);
    HUEtoRGB(bottomcolor, bottom);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();

    draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize(), pos, IM_COL32(top.r, top.g, top.b, 255), firstcolor.c_str());

    float firstWidth = ImGui::CalcTextSize(firstcolor.c_str()).x;
    pos.x += firstWidth;

    draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize(), pos, IM_COL32(bottom.r, bottom.g, bottom.b, 255), secondcolor.c_str());
}

float CImguiUtils::GetCvarFloat(const char* name)
{
    cvar_t* cvar = gEngfuncs.pfnGetCvarPointer(name);
    return cvar->value;
}

void CImguiUtils::SetCvarFloat(const char* name, float value)
{
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "%s %f", name, value);
    gEngfuncs.pfnClientCmd(cmd);
}

void CImguiUtils::GetCvarColor(const char* name, float outColor[3])
{
    const char* str = gEngfuncs.pfnGetCvarString(name);
    int r, g, b;

    sscanf(str, "%d %d %d", &r, &g, &b);

    outColor[0] = r / 255.0f;
    outColor[1] = g / 255.0f;
    outColor[2] = b / 255.0f;
}

void CImguiUtils::SetCvarColor(const char* name, const float color[3])
{
    int r = (int)(color[0] * 255.0f + 0.5f);
    int g = (int)(color[1] * 255.0f + 0.5f);
    int b = (int)(color[2] * 255.0f + 0.5f);

    char cmd[64];
    snprintf(cmd, sizeof(cmd), "%s \"%d %d %d\"", name, r, g, b);
    gEngfuncs.pfnClientCmd(cmd);
}
