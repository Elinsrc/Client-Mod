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
};
