#pragma once
#include "imgui.h"

struct RGBColor {
    int r, g, b;
};

class CImguiUtils
{
public:
    ImVec4 ColorFromCode(char code);
    void TextWithColorCodes(const char* text);
    void HUEtoRGB(float hue, RGBColor &color);
    void DrawModelName(float topcolor, float bottomcolor, const char* model);
    float GetCvarFloat(const char* name);
    void SetCvarFloat(const char* name, float value);
    void GetCvarColor(const char* name, float outColor[3]);
    void SetCvarColor(const char* name, const float color[3]);
};
