#include <cmath>
#include <cstring>
#include <cstdio>

#include "hud.h"
#include "cl_util.h"
#include "cl_entity.h"
#include "hud_debug.h"
#include "pm_defs.h"
#include "event_api.h"


// https://github.com/SNMetamorph/goldsrc-monitor
extern "C"
{
    extern playermove_t *pmove;
}


int CHudDebug::Init()
{
    m_iFlags = HUD_ACTIVE;

    cl_debug = CVAR_CREATE("cl_debug", "0", FCVAR_ARCHIVE);
    cl_debug_showfps = CVAR_CREATE("cl_debug_showfps", "1", FCVAR_ARCHIVE);

    gHUD.AddHudElem(this);
    return 0;
}


int CHudDebug::VidInit()
{
    return 1;
}


static const unsigned char colors[4][3] = {
    {255, 0, 0},
    {255, 255, 0},
    {0, 255, 0},
    {255, 255, 255}
};


const unsigned char* getColorForFPS(int fps)
{
    if (fps > 59)
        return colors[2];
    else if (fps > 29)
        return colors[1];
    else
        return colors[0];
}


extern struct RGBColor {
    int r;
    int g;
    int b;
} top, bottom;


void CHudDebug::ClientModelName(cl_entity_s *entity, int x, int y, int r, int g, int b)
{
    const char* model = g_PlayerInfoList[entity->index].model;

    if (model)
    {
        const char* model_str = "Model: ";
        size_t modelLength = strlen(model);
        size_t mid = modelLength / 2;

        char firstcolor[256];
        char secondcolor[256];

        strncpy(firstcolor, model, mid);
        firstcolor[mid] = '\0';
        strncpy(secondcolor, model + mid, modelLength - mid);
        secondcolor[modelLength - mid] = '\0';

        gHUD.HUEtoRGB(g_PlayerInfoList[entity->index].topcolor, top.r, top.g, top.b);
        gHUD.HUEtoRGB(g_PlayerInfoList[entity->index].bottomcolor, bottom.r, bottom.g, bottom.b);

        int modelWidth, modelHeight;
        gHUD.GetConsoleStringSizeWithColorTags(const_cast<char*>(model_str), modelWidth, modelHeight);

        int firstColorWidth, firstColorHeight;
        gHUD.GetConsoleStringSizeWithColorTags(firstcolor, firstColorWidth, firstColorHeight);

        int secondColorWidth, secondColorHeight;
        gHUD.GetConsoleStringSizeWithColorTags(secondcolor, secondColorWidth, secondColorHeight);

        gHUD.DrawHudText(x, y, model_str, r, g, b);
        gHUD.DrawHudText(x + modelWidth, y, firstcolor, top.r, top.g, top.b);
        gHUD.DrawHudText(x + modelWidth + firstColorWidth, y, secondcolor, bottom.r, bottom.g, bottom.b);
    }
    else
    {
        gHUD.DrawHudText(x, y, "Model: Unknown", r, g, b);
    }
}


void CHudDebug::CurrentClientInfo(int r, int g, int b)
{
    char str[256];
    int xpos = ScreenWidth / 1.4;

    cl_entity_t *localPlayer = gEngfuncs.GetLocalPlayer();
    float velocityNum = std::round(std::hypot(pmove->velocity[0], pmove->velocity[1]));

    sprintf(str, "Velocity: %.2f u/s (%.2f, %.2f, %.2f)", velocityNum, pmove->velocity.x, pmove->velocity.y, pmove->velocity.z);
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 13, str, r, g, b);

    sprintf(str, "Origin: (%.2f, %.2f, %.2f)", localPlayer->origin.x, localPlayer->origin.y, localPlayer->origin.z);
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 14, str, r, g, b);

    sprintf(str, "Angles: (%.2f, %.2f, %.2f)", localPlayer->angles.x, localPlayer->angles.y, localPlayer->angles.z);
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 15, str, r, g, b);

    sprintf(str, "Movetype: %s", m_CustomUtils.GetMovetypeName(localPlayer->curstate.movetype));
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 16, str, r, g, b);

    vec3_t viewOffset;
    gEngfuncs.pEventAPI->EV_LocalPlayerViewheight(viewOffset);

    sprintf(str, "View Offset: (%.2f, %.2f, %.2f)", viewOffset.x, viewOffset.y, viewOffset.z);
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 18, str, r, g, b);

    const vec3_t &punchAngle = pmove->punchangle;

    sprintf(str, "Punch Angle: (%.2f, %.2f, %.2f)", punchAngle.x, punchAngle.y, punchAngle.z);
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 19, str, r, g, b);

    sprintf(str, "Player Flags: %d", pmove->flags);
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 20, str, r, g, b);

    sprintf(str, "Hull Type: %d", pmove->usehull);
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 21, str, r, g, b);

    sprintf(str, "Gravity: %.2f", pmove->gravity);
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 22, str, r, g, b);

    sprintf(str, "Friction: %.2f", pmove->friction);
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 23, str, r, g, b);

    sprintf(str, "On Ground: %s", pmove->onground ? "yes" : "no");
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 24, str, r, g, b);

    sprintf(str, "Anim. Frame: %.1f", localPlayer->curstate.frame);
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 26, str, r, g, b);

    sprintf(str, "Anim. Sequence: %d", localPlayer->curstate.sequence);
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 27, str, r, g, b);

    sprintf(str, "Bodygroup Number: %d", localPlayer->curstate.body);
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 28, str, r, g, b);

    sprintf(str, "Skin Number: %d", localPlayer->curstate.skin);
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 29, str, r, g, b);

    sprintf(str, "RenderMode: %s", m_CustomUtils.GetRenderModeName(localPlayer->curstate.rendermode));
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 31, str, r, g, b);

    sprintf(str, "RenderFx: %s", m_CustomUtils.GetRenderFxName(localPlayer->curstate.renderfx));
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 32, str, r, g, b);

    sprintf(str, "Render Amount: %d", localPlayer->curstate.renderamt);
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 33, str, r, g, b);

    sprintf(str, "Render Color: %d %d %d", localPlayer->curstate.rendercolor.r, localPlayer->curstate.rendercolor.g, localPlayer->curstate.rendercolor.b);
    gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 34, str, r, g, b);
}


void CHudDebug::AllClientsInfo(int r, int g, int b, int ClientIndex )
{
    char str[256];
    cl_entity_t *localPlayer = gEngfuncs.GetLocalPlayer();

    if(!ClientIndex)
        return;

    cl_entity_t *pClient = gEngfuncs.GetEntityByIndex(ClientIndex);

    Vector Top = Vector(pClient->origin.x, pClient->origin.y, pClient->origin.z + pClient->curstate.mins.z);
    Vector Bot = Vector(pClient->origin.x, pClient->origin.y, pClient->origin.z + pClient->curstate.maxs.z);

    float ScreenTop[3], ScreenBot[3];

    bool m_bScreenTop = m_CustomUtils.CalcScreen(Top, ScreenTop);
    bool m_bScreenBot = m_CustomUtils.CalcScreen(Bot, ScreenBot);

    if (m_bScreenTop && m_bScreenBot)
    {
        float Height = ScreenBot[1] - ScreenTop[1];
        int y = Height + ScreenTop[1];
        int x = ScreenTop[0] - (Height * 0.4f);

        sprintf(str, "%s", g_PlayerInfoList[ClientIndex].name);
        gHUD.DrawHudTextCentered(ScreenTop[0], y - 18, str, r, g, b);

        sprintf(str, "Index: %d", pClient->index);
        gHUD.DrawHudText(x, y, str, r, g, b);

        ClientModelName(pClient, x, y + 20, r, g, b);

        sprintf(str, "Velocity: %.2f u/s", m_CustomUtils.GetEntityVelocityApprox(pClient, 22).Length2D());
        gHUD.DrawHudText(x, y + 60, str, r, g, b);

        sprintf(str, "Origin: (%.2f, %.2f, %.2f)", pClient->origin.x, pClient->origin.y, pClient->origin.z);
        gHUD.DrawHudText(x, y + 80, str, r, g, b);

        sprintf(str, "Angles: (%.2f, %.2f, %.2f)", pClient->angles.x, pClient->angles.y, pClient->angles.z);
        gHUD.DrawHudText(x, y + 100, str, r, g, b);

        sprintf(str, "Movetype: %s", m_CustomUtils.GetMovetypeName(pClient->curstate.movetype));
        gHUD.DrawHudText(x, y + 120, str, r, g, b);

        Vector vDiff = localPlayer->origin - pClient->origin;
        sprintf(str, "Distance: %.2f", vDiff.Length());
        gHUD.DrawHudText(x, y + 140, str, r, g, b);

        sprintf(str, "Anim. Frame: %.1f", pClient->curstate.frame);
        gHUD.DrawHudText(x, y + 180, str, r, g, b);

        sprintf(str, "Anim. Sequence: %d", pClient->curstate.sequence);
        gHUD.DrawHudText(x, y + 200, str, r, g, b);

        sprintf(str, "Bodygroup Number: %d", pClient->curstate.body);
        gHUD.DrawHudText(x, y + 220, str, r, g, b);

        sprintf(str, "Skin Number: %d", pClient->curstate.skin);
        gHUD.DrawHudText(x, y + 240, str, r, g, b);

        sprintf(str, "Render Mode: %s", m_CustomUtils.GetRenderModeName(pClient->curstate.rendermode));
        gHUD.DrawHudText(x, y + 280, str, r, g, b);

        sprintf(str, "Render Fx: %s", m_CustomUtils.GetRenderFxName(pClient->curstate.renderfx));
        gHUD.DrawHudText(x, y + 300, str, r, g, b);

        sprintf(str, "Render Amount: %d", pClient->curstate.renderamt);
        gHUD.DrawHudText(x, y + 320, str, r, g, b);

        sprintf(str, "Render Color: %d %d %d", pClient->curstate.rendercolor.r, pClient->curstate.rendercolor.g, pClient->curstate.rendercolor.b);
        gHUD.DrawHudText(x, y + 340, str, r, g, b);
        m_CustomUtils.DrawBoxCornerOutline(ScreenTop[0] - (Height * 0.25f), ScreenTop[1], Height / 2, Height, 1, 0, 255, 0, 255);
    }
}


int CHudDebug::Draw(float flTime)
{
    gHUD.GetAllPlayersInfo();

    int xpos = ScreenWidth / 1.4;

    int DebugMode = cl_debug->value;

    float fps = m_CustomUtils.GetFrameTime();

    const unsigned char* color = getColorForFPS(static_cast<int>(fps));
    int r = color[0];
    int g = color[1];
    int b = color[2];

    char str[256];

    if (DebugMode > 0.0f || cl_debug_showfps->value > 0.0f) {
        sprintf(str, "FPS: %.0f", fps);
        gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight, str, r, g, b);

        sprintf(str, "Frame Time: %.0f ms\n", 1000.f / fps);
        gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 2, str, r, g, b);
    }

    r = colors[3][0];
    g = colors[3][1];
    b = colors[3][2];

    cl_entity_t *localPlayer = gEngfuncs.GetLocalPlayer();

    int ClientIndex;
    const float lineLen = 11590.0f;
    vec3_t intersectPoint;
    vec3_t viewOrigin = localPlayer->origin;
    vec3_t viewAngles, viewDir;

    gEngfuncs.GetViewAngles(viewAngles);
    gEngfuncs.pfnAngleVectors(viewAngles, viewDir, nullptr, nullptr);

    if( DebugMode == 3.0f)
    {
        if( g_iUser1 )
            ClientIndex = g_iUser2;
        else
            ClientIndex = m_CustomUtils.TraceEntity(viewOrigin, viewDir, lineLen, intersectPoint);

        cl_entity_t *pClient = gEngfuncs.GetEntityByIndex(ClientIndex);

        if (!m_CustomUtils.CheckForPlayer(pClient))
            ClientIndex = 0;
    }

    const char* debugMode = "Debug mode: ";

    int debugWidth, debugHeight;
    gHUD.GetConsoleStringSizeWithColorTags((char*)debugMode, debugWidth, debugHeight);

    if (DebugMode > 0.0f) {
        sprintf(str, "Map: %s", gEngfuncs.pfnGetLevelName());
        gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 4, str, r, g, b);

        float clientTime = gEngfuncs.GetClientTime();
        sprintf(str, "Map Time: %s", m_CustomUtils.FormatTime(clientTime));
        gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 5, str, r, g, b);

        sprintf(str, "Name: %s", g_PlayerInfoList[localPlayer->index].name);
        gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 7, str, r, g, b);

        sprintf(str, "Index: %d", localPlayer->index);
        gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 8, str, r, g, b);

        ClientModelName(localPlayer, xpos, gHUD.m_scrinfo.iCharHeight * 9, r, g, b);

        gHUD.DrawHudText(xpos, gHUD.m_scrinfo.iCharHeight * 11, debugMode, r, g, b);
    }

    switch (DebugMode) {
        case 0:
            break;
        case 1:
            gHUD.DrawHudText(xpos + debugWidth, gHUD.m_scrinfo.iCharHeight * 11, "1 (Minimal Info)", 0, 255, 0);
            break;
        case 2:
            gHUD.DrawHudText(xpos + debugWidth, gHUD.m_scrinfo.iCharHeight * 11, "2 (Current Client Info)", 0, 255, 0);
            CurrentClientInfo(r, g, b);
            break;
        case 3:
            gHUD.DrawHudText(xpos + debugWidth, gHUD.m_scrinfo.iCharHeight * 11, "3 (All Clients Info)", 0, 255, 0);
            if(!ClientIndex)
                gHUD.DrawHudTextCentered(ScreenWidth / 2, ScreenWidth / 3, "Aim at a player or follow them in 3rd person observer mode to gain information!", 255, 0, 0);
            else
                AllClientsInfo(r, g, b, ClientIndex);
            break;
        case 4:
            gHUD.DrawHudText(xpos + debugWidth, gHUD.m_scrinfo.iCharHeight * 11, "4 (Entity Info) UNDONE", 255, 0, 0);
            break;
        default:
            gHUD.DrawHudText(xpos + debugWidth, gHUD.m_scrinfo.iCharHeight * 11, "(UNKNOWN)", 255, 0, 0);
            break;
    }

    return 0;
}
