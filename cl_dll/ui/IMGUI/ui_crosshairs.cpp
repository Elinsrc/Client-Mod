#include "hud.h"
#include "cl_util.h"
#include "imgui.h"
#include "ui_crosshairs.h"

cvar_t* cl_cross;
cvar_t* cl_cross_color;
cvar_t* cl_cross_alpha;
cvar_t* cl_cross_gap;
cvar_t* cl_cross_size;
cvar_t* cl_cross_thickness;
cvar_t* cl_cross_outline;
cvar_t* cl_cross_top_line;
cvar_t* cl_cross_bottom_line;
cvar_t* cl_cross_left_line;
cvar_t* cl_cross_right_line;
cvar_t* cl_cross_dot_size;
cvar_t* cl_cross_dot_color;
cvar_t* cl_cross_circle_radius;

bool CImGuiCrosshairs::m_ShowCrosshairs = false;

void CImGuiCrosshairs::Initialize()
{
    cl_cross = CVAR_CREATE("cl_cross", "0", FCVAR_ARCHIVE);
	cl_cross_color = CVAR_CREATE("cl_cross_color", "0 255 0", FCVAR_ARCHIVE);
	cl_cross_alpha = CVAR_CREATE("cl_cross_alpha", "200", FCVAR_ARCHIVE);
	cl_cross_thickness = CVAR_CREATE("cl_cross_thickness", "2", FCVAR_ARCHIVE);
	cl_cross_size = CVAR_CREATE("cl_cross_size", "10", FCVAR_ARCHIVE);
	cl_cross_gap =  CVAR_CREATE("cl_cross_gap", "3", FCVAR_ARCHIVE);
	cl_cross_outline = CVAR_CREATE("cl_cross_outline", "0", FCVAR_ARCHIVE);
	cl_cross_circle_radius = CVAR_CREATE("cl_cross_circle_radius", "0", FCVAR_ARCHIVE);
	cl_cross_dot_size = CVAR_CREATE("cl_cross_dot_size", "0", FCVAR_ARCHIVE);
	cl_cross_dot_color = CVAR_CREATE("cl_cross_dot_color", "", FCVAR_ARCHIVE);
	cl_cross_top_line = CVAR_CREATE("cl_cross_top_line", "1", FCVAR_ARCHIVE);
	cl_cross_bottom_line = CVAR_CREATE("cl_cross_bottom_line", "1", FCVAR_ARCHIVE);
	cl_cross_left_line = CVAR_CREATE("cl_cross_left_line", "1", FCVAR_ARCHIVE);
	cl_cross_right_line = CVAR_CREATE("cl_cross_right_line", "1", FCVAR_ARCHIVE);
}

void CImGuiCrosshairs::VidInitialize()
{
}

void CImGuiCrosshairs::Terminate()
{
}

void CImGuiCrosshairs::Think()
{
}

void CImGuiCrosshairs::Draw()
{
    if (!m_ShowCrosshairs || cl_cross->value == 0.0f)
        return;

    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    float alpha = cl_cross_alpha->value;

    int r = 0, g = 255, b = 0;
    sscanf(cl_cross_color->string, "%d %d %d", &r, &g, &b);
    ImU32 crossColor = IM_COL32(r, g, b, (int)alpha);

    int dot_r = r, dot_g = g, dot_b = b;
    sscanf(cl_cross_dot_color->string, "%d %d %d", &dot_r, &dot_g, &dot_b);
    ImU32 dotColor = IM_COL32(dot_r, dot_g, dot_b, (int)alpha);

    float xPos = ScreenWidth * 0.5f;
    float yPos = ScreenHeight * 0.5f;

    float size   = cl_cross_size->value;
    float thickness = cl_cross_thickness->value;
    float gap    = cl_cross_gap->value;
    float outline = cl_cross_outline->value;
    float dotsize = cl_cross_dot_size->value;

    ImU32 black = IM_COL32(0, 0, 0, (int)alpha);

    auto drawRect = [&](float x1, float y1, float x2, float y2, ImU32 color)
    {
        draw->AddRectFilled(ImVec2(roundf(x1), roundf(y1)), ImVec2(roundf(x2), roundf(y2)), color);
    };

    if (outline > 0.0f)
    {
        if (cl_cross_top_line->value)
            drawRect(xPos - gap/2 - outline, yPos - size - thickness - outline, xPos + gap/2 + outline, yPos - size + outline, black);

        if (cl_cross_bottom_line->value)
            drawRect(xPos - gap/2 - outline, yPos + size - outline, xPos + gap/2 + outline, yPos + size + thickness + outline, black);

        if (cl_cross_left_line->value)
            drawRect(xPos - size - thickness - outline, yPos - gap/2 - outline, xPos - size + outline, yPos + gap/2 + outline, black);

        if (cl_cross_right_line->value)
            drawRect(xPos + size - outline, yPos - gap/2 - outline, xPos + size + thickness + outline, yPos + gap/2 + outline, black);

        if (dotsize > 0.0f)
            drawRect(xPos - dotsize/2 - outline, yPos - dotsize/2 - outline, xPos + dotsize/2 + outline, yPos + dotsize/2 + outline, black);
    }

    if (cl_cross_top_line->value)
        drawRect(xPos - gap/2, yPos - size - thickness, xPos + gap/2, yPos - size, crossColor);

    if (cl_cross_bottom_line->value)
        drawRect(xPos - gap/2, yPos + size, xPos + gap/2, yPos + size + thickness, crossColor);

    if (cl_cross_left_line->value)
        drawRect(xPos - size - thickness, yPos - gap/2, xPos - size, yPos + gap/2, crossColor);

    if (cl_cross_right_line->value)
        drawRect(xPos + size, yPos - gap/2, xPos + size + thickness, yPos + gap/2, crossColor);

    if (dotsize > 0.0f)
        drawRect(xPos - dotsize/2, yPos - dotsize/2, xPos + dotsize/2, yPos + dotsize/2, dotColor);

    if (cl_cross_circle_radius->value > 0.0f)
    {
        float radius = cl_cross_circle_radius->value;
        draw->AddCircle(ImVec2(xPos, yPos), radius, crossColor, 64, 1.0f);
    }
}

bool CImGuiCrosshairs::Active()
{
    return m_ShowCrosshairs;
}

bool CImGuiCrosshairs::CursorRequired()
{
    return false;
}

bool CImGuiCrosshairs::HandleKey(bool keyDown, int keyNumber, const char *bindName)
{
    return true;
}
