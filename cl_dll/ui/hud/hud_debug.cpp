#include <cmath>
#include <string>
#include <cstring>
#include <cstdio>
#include <algorithm>

#include "hud.h"
#include "cl_util.h"
#include "cl_entity.h"
#include "parsemsg.h"
#include "hud_debug.h"
#include "pm_defs.h"
#include "event_api.h"
#include "triangleapi.h"

#ifdef _WIN32
#define HSPRITE DeletedWinapi_HSPRITE // prevent windows.h from defining it
#include <windows.h>
#undef HSPRITE
typedef int HSPRITE;
#else
#include <time.h>
#endif

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

static float GetCurrentSysTime()
{
#ifdef _WIN32
    static LARGE_INTEGER perfFreq;
    static LARGE_INTEGER clockStart;
    LARGE_INTEGER currentTime;
    LONGLONG timeDiff;

    if (!perfFreq.QuadPart)
    {
        QueryPerformanceFrequency(&perfFreq);
        QueryPerformanceCounter(&clockStart);
    }

    QueryPerformanceCounter(&currentTime);
    timeDiff = currentTime.QuadPart - clockStart.QuadPart;
    return static_cast<float>(timeDiff) / static_cast<float>(perfFreq.QuadPart);
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec / 1000000000.0;
#endif
}

float CHudDebug::GetFrametime()
{
    const float smoothFactor = 0.24f;
    const float diffThreshold = 0.13f;
    float currSysTime = GetCurrentSysTime();
    float timeDelta = currSysTime - m_lastSysTime;

    if ((timeDelta - m_lastFrameTime) > diffThreshold)
        timeDelta = m_lastFrameTime;

    m_frameTime += (timeDelta - m_frameTime) * smoothFactor;
    m_lastFrameTime = m_frameTime;
    m_lastSysTime = currSysTime;
    return m_frameTime;
}

const char *CHudDebug::GetMovetypeName(int moveType)
{
    switch (moveType)
    {
        case MOVETYPE_BOUNCE:           return "Bounce";
        case MOVETYPE_BOUNCEMISSILE:    return "Bounce-missile";
        case MOVETYPE_FLY:              return "Fly";
        case MOVETYPE_FLYMISSILE:       return "Fly-missile";
        case MOVETYPE_FOLLOW:           return "Follow";
        case MOVETYPE_NOCLIP:           return "Noclip";
        case MOVETYPE_NONE:             return "None";
        case MOVETYPE_PUSH:             return "Push";
        case MOVETYPE_PUSHSTEP:         return "Push-step";
        case MOVETYPE_STEP:             return "Step";
        case MOVETYPE_TOSS:             return "Toss";
        case MOVETYPE_WALK:             return "Walk";
        default:                        return "Unknown";
    }
}

const char *CHudDebug::GetRenderModeName(int renderMode)
{
    switch (renderMode)
    {
        case kRenderNormal:         return "Normal";
        case kRenderTransColor:     return "Trans. color";
        case kRenderTransTexture:   return "Trans. texture";
        case kRenderGlow:           return "Glow";
        case kRenderTransAlpha:     return "Trans. alpha";
        case kRenderTransAdd:       return "Trans. additive";
        default:                    return "Unknown";
    }
}

const char *CHudDebug::GetRenderFxName(int renderFx)
{
    switch (renderFx)
    {
        case kRenderFxNone:             return "None";
        case kRenderFxPulseSlow:        return "Pulse (slow)";
        case kRenderFxPulseFast:        return "Pulse (fast)";
        case kRenderFxPulseSlowWide:    return "Pulse (slow wide)";
        case kRenderFxPulseFastWide:    return "Pulse (fast wide)";
        case kRenderFxFadeSlow:         return "Fade (slow)";
        case kRenderFxFadeFast:         return "Fade (fast)";
        case kRenderFxSolidSlow:        return "Solid (slow)";
        case kRenderFxSolidFast:        return "Solid (fast)";
        case kRenderFxStrobeSlow:       return "Strobe (slow)";
        case kRenderFxStrobeFast:       return "Strobe (fast)";
        case kRenderFxStrobeFaster:     return "Strobe (faster)";
        case kRenderFxFlickerSlow:      return "Flicker (slow)";
        case kRenderFxFlickerFast:      return "Flicker (fast)";
        case kRenderFxNoDissipation:    return "No dissipation";
        case kRenderFxDistort:          return "Distort";
        case kRenderFxHologram:         return "Hologram";
        case kRenderFxDeadPlayer:       return "Dead player";
        case kRenderFxExplode:          return "Explode";
        case kRenderFxGlowShell:        return "Glow shell";
        case kRenderFxClampMinScale:    return "Clamp min. scale";
        case kRenderFxLightMultiplier:  return "Light multiplier";
        default:                        return "Unknown";
    }
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

void CHudDebug::formatTime(float totalSeconds, char* output)
{
    int seconds = static_cast<int>(totalSeconds);
    int days = seconds / (24 * 3600);
    seconds %= (24 * 3600);
    int hours = seconds / 3600;
    seconds %= 3600;
    int minutes = seconds / 60;
    seconds %= 60;

    char buffer[256] = "";
    if (days > 0)
    {
        sprintf(buffer + strlen(buffer), "%dd ", days);
    }
    if (hours > 0)
    {
        sprintf(buffer + strlen(buffer), "%dh ", hours);
    }
    if (minutes > 0)
    {
        sprintf(buffer + strlen(buffer), "%dm ", minutes);
    }
    sprintf(buffer + strlen(buffer), "%ds", seconds);

    sprintf(output, "Map Time: %s", buffer);
}

extern struct RGBColor {
    int r;
    int g;
    int b;
} top, bottom;

void CHudDebug::Box(int x, int y, int w, int h, int linewidth, int r, int g, int b, int a)
{
    FillRGBABlend(x, y, w, linewidth, r, g, b, a);
    FillRGBABlend(x + w - linewidth, y + linewidth, linewidth, h - linewidth, r, g, b, a);
    FillRGBABlend(x, y + linewidth, linewidth, h - linewidth, r, g, b, a);
    FillRGBABlend(x + linewidth, y + h - linewidth, w - linewidth * 2, linewidth, r, g, b, a);
}

void CHudDebug::BoxOutline(float x, float y, float w, float h, float linewidth, int r, int g, int b, int a)
{
    Box(x, y, w, h, linewidth, r, g, b, a);
    Box(x - 1, y - 1, w + 2, h + 2, 1, 0, 0, 0, a);
    Box(x + linewidth, y + linewidth, w - linewidth * 2, h - linewidth * 2, 1, 0, 0, 0, a);
}

void CHudDebug::BoxCorner(int x, int y, int w, int h, int linewidth, int r, int g, int b, int a)
{
    FillRGBABlend(x, y, w / 4, linewidth, r, g, b, a);
    FillRGBABlend(x + w - w / 4, y, w / 4, linewidth, r, g, b, a);
    FillRGBABlend(x, y + linewidth, linewidth, h / 4 - linewidth, r, g, b, a);
    FillRGBABlend(x, y + h - h / 4, linewidth, h / 4, r, g, b, a);
    FillRGBABlend(x + w - linewidth, y + linewidth, linewidth, h / 4 - linewidth, r, g, b, a);
    FillRGBABlend(x + w - linewidth, y + h - h / 4, linewidth, h / 4, r, g, b, a);
    FillRGBABlend(x + linewidth, y + h - linewidth, w / 4 - linewidth, linewidth, r, g, b, a);
    FillRGBABlend(x + w - w / 4, y + h - linewidth, w / 4 - linewidth, linewidth, r, g, b, a);
}

void CHudDebug::BoxCornerOutline(int x, int y, int w, int h, int linewidth, int r, int g, int b, int a)
{
    BoxCorner(x - 1, y + 1, w, h, linewidth, 0, 0, 0, a);
    BoxCorner(x - 1, y - 1, w, h, linewidth, 0, 0, 0, a);
    BoxCorner(x + 1, y + 1, w, h, linewidth, 0, 0, 0, a);
    BoxCorner(x + 1, y - 1, w, h, linewidth, 0, 0, 0, a);

    BoxCorner(x, y, w, h, linewidth, r, g, b, a);
}

bool CHudDebug::CalcScreen(float *Origin, float *VecScreen) {
    int iResult = gEngfuncs.pTriAPI->WorldToScreen(Origin, VecScreen);

    if (VecScreen[0] < 1 && VecScreen[1] < 1 && VecScreen[0] > -1 && VecScreen[1] > -1 && !iResult)
    {
        VecScreen[0] = VecScreen[0] * (ScreenWidth / 2) + (ScreenWidth / 2);
        VecScreen[1] = -VecScreen[1] * (ScreenHeight / 2) + (ScreenHeight / 2);

        return true;
    }

    return false;
}

void CHudDebug::ClientModelName(cl_entity_s *entity, int x, int y, int r, int g, int b)
{
    std::string firstcolor;
    std::string secondcolor;
    const char* model = g_PlayerInfoList[entity->index].model;

    if (model)
    {
        std::string modelname = model;
        std::string model_str = "Model: ";

        size_t mid = modelname.length() / 2;
        firstcolor = modelname.substr(0, mid);
        secondcolor = modelname.substr(mid);

        gHUD.HUEtoRGB(g_PlayerInfoList[entity->index].topcolor, top.r, top.g, top.b);
        gHUD.HUEtoRGB(g_PlayerInfoList[entity->index].bottomcolor, bottom.r, bottom.g, bottom.b);

        int ModelWidth = gHUD.GetHudStringWidth(model_str.c_str());
        int firstcolorWidth = gHUD.GetHudStringWidth(firstcolor.c_str());

        gHUD.DrawHudText(x, y, model_str.c_str(), r, g, b);
        gHUD.DrawHudText(x + ModelWidth, y, firstcolor.c_str(), top.r, top.g, top.b);
        gHUD.DrawHudText(x + ModelWidth + firstcolorWidth, y, secondcolor.c_str(), bottom.r, bottom.g, bottom.b);
    }
    else
    {
        gHUD.DrawHudText(x, y, "Model: Unknown", r, g, b);
    }
}

void CHudDebug::CurrentClientInfo(int r, int g, int b)
{
    char str[256];

    cl_entity_t *localPlayer = gEngfuncs.GetLocalPlayer();
    float velocityNum = std::round(std::hypot(pmove->velocity[0], pmove->velocity[1]));

    sprintf(str, "Velocity: %.2f u/s (%.2f, %.2f, %.2f)", velocityNum, pmove->velocity.x, pmove->velocity.y, pmove->velocity.z);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 13, str, r, g, b);

    sprintf(str, "Origin: (%.2f, %.2f, %.2f)", localPlayer->origin.x, localPlayer->origin.y, localPlayer->origin.z);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 14, str, r, g, b);

    sprintf(str, "Angles: (%.2f, %.2f, %.2f)", localPlayer->angles.x, localPlayer->angles.y, localPlayer->angles.z);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 15, str, r, g, b);

    sprintf(str, "Movetype: %s", GetMovetypeName(localPlayer->curstate.movetype));
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 16, str, r, g, b);

    vec3_t viewOffset;
    gEngfuncs.pEventAPI->EV_LocalPlayerViewheight(viewOffset);

    sprintf(str, "View Offset: (%.2f, %.2f, %.2f)", viewOffset.x, viewOffset.y, viewOffset.z);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 18, str, r, g, b);

    const vec3_t &punchAngle = pmove->punchangle;

    sprintf(str, "Punch Angle: (%.2f, %.2f, %.2f)", punchAngle.x, punchAngle.y, punchAngle.z);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 19, str, r, g, b);

    sprintf(str, "Player Flags: %d", pmove->flags);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 20, str, r, g, b);

    sprintf(str, "Hull Type: %d", pmove->usehull);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 21, str, r, g, b);

    sprintf(str, "Gravity: %.2f", pmove->gravity);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 22, str, r, g, b);

    sprintf(str, "Friction: %.2f", pmove->friction);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 23, str, r, g, b);

    sprintf(str, "On Ground: %s", pmove->onground ? "yes" : "no");
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 24, str, r, g, b);

    sprintf(str, "Anim. Frame: %.1f", localPlayer->curstate.frame);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 26, str, r, g, b);

    sprintf(str, "Anim. Sequence: %d", localPlayer->curstate.sequence);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 27, str, r, g, b);

    sprintf(str, "Bodygroup Number: %d", localPlayer->curstate.body);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 28, str, r, g, b);

    sprintf(str, "Skin Number: %d", localPlayer->curstate.skin);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 29, str, r, g, b);

    sprintf(str, "RenderMode: %s", GetRenderModeName(localPlayer->curstate.rendermode));
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 31, str, r, g, b);

    sprintf(str, "RenderFx: %s", GetRenderFxName(localPlayer->curstate.renderfx));
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 32, str, r, g, b);

    sprintf(str, "Render Amount: %d", localPlayer->curstate.renderamt);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 33, str, r, g, b);

    sprintf(str, "Render Color: %d %d %d", localPlayer->curstate.rendercolor.r, localPlayer->curstate.rendercolor.g, localPlayer->curstate.rendercolor.b);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 34, str, r, g, b);
}

bool CHudDebug::CheckForClient(cl_entity_s *pEnt) {
    return pEnt && pEnt->model && pEnt->model->name && pEnt->player;
}

vec3_t CHudDebug::GetEntityVelocityApprox(cl_entity_t *entity, int approxStep)
{
    if (entity)
    {
        const int currIndex = entity->current_position;
        position_history_t &currState = entity->ph[currIndex & HISTORY_MASK];
        position_history_t &prevState = entity->ph[(currIndex - approxStep) & HISTORY_MASK];
        float timeDelta = currState.animtime - prevState.animtime;

        if (fabs(timeDelta) > 0.0f) {
            vec3_t originDelta = currState.origin - prevState.origin;
            return originDelta / timeDelta;
        }
    }
    return vec3_t(0, 0, 0);
}

void CHudDebug::TraceLine(vec3_t &origin, vec3_t &dir, float lineLen, pmtrace_t *traceData)
{
    vec3_t lineStart;
    vec3_t lineEnd;
    cl_entity_t *localPlayer;

    lineStart   = origin;
    lineEnd     = lineStart + (dir * lineLen);
    localPlayer = gEngfuncs.GetLocalPlayer();

    gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction(false, true);
    gEngfuncs.pEventAPI->EV_PushPMStates();
    gEngfuncs.pEventAPI->EV_SetSolidPlayers(localPlayer->index - 1);
    gEngfuncs.pEventAPI->EV_SetTraceHull(2);
    gEngfuncs.pEventAPI->EV_PlayerTrace(
        lineStart, lineEnd, PM_NORMAL,
        -1, traceData
    );
    gEngfuncs.pEventAPI->EV_PopPMStates();
}

int CHudDebug::TraceEntity(vec3_t origin, vec3_t dir, float distance, vec3_t &intersect)
{
    pmtrace_t traceData;

    TraceLine(origin, dir, distance, &traceData);
    intersect = origin + dir * distance * traceData.fraction;
    if (traceData.fraction < 1.f)
    {
        if (traceData.ent > 0)
            return gEngfuncs.pEventAPI->EV_IndexFromTrace(&traceData);
    }
    return 0;
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

    bool m_bScreenTop = CalcScreen(Top, ScreenTop);
    bool m_bScreenBot = CalcScreen(Bot, ScreenBot);

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

        sprintf(str, "Velocity: %.2f u/s", GetEntityVelocityApprox(pClient, 22).Length2D());
        gHUD.DrawHudText(x, y + 60, str, r, g, b);

        sprintf(str, "Origin: (%.2f, %.2f, %.2f)", pClient->origin.x, pClient->origin.y, pClient->origin.z);
        gHUD.DrawHudText(x, y + 80, str, r, g, b);

        sprintf(str, "Angles: (%.2f, %.2f, %.2f)", pClient->angles.x, pClient->angles.y, pClient->angles.z);
        gHUD.DrawHudText(x, y + 100, str, r, g, b);

        sprintf(str, "Movetype: %s", GetMovetypeName(pClient->curstate.movetype));
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

        sprintf(str, "Render Mode: %s", GetRenderModeName(pClient->curstate.rendermode));
        gHUD.DrawHudText(x, y + 280, str, r, g, b);

        sprintf(str, "Render Fx: %s", GetRenderFxName(pClient->curstate.renderfx));
        gHUD.DrawHudText(x, y + 300, str, r, g, b);

        sprintf(str, "Render Amount: %d", pClient->curstate.renderamt);
        gHUD.DrawHudText(x, y + 320, str, r, g, b);

        sprintf(str, "Render Color: %d %d %d", pClient->curstate.rendercolor.r, pClient->curstate.rendercolor.g, pClient->curstate.rendercolor.b);
        gHUD.DrawHudText(x, y + 340, str, r, g, b);
        BoxCornerOutline(ScreenTop[0] - (Height * 0.25f), ScreenTop[1], Height / 2, Height, 1, 0, 255, 0, 255);
    }
}

int CHudDebug::Draw(float flTime)
{
    gHUD.GetAllPlayersInfo();

    int DebugMode = cl_debug->value;

    float timeDelta = GetFrametime();
    float fps = 1.f / timeDelta;
    char str[256];

    const unsigned char* color = getColorForFPS(static_cast<int>(fps));
    int r = color[0];
    int g = color[1];
    int b = color[2];

    if (DebugMode > 0.0f || cl_debug_showfps->value > 0.0f) {
        sprintf(str, "FPS: %.1f", fps);
        gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight, str, r, g, b);

        sprintf(str, "Frame Time: %.1f ms\n", timeDelta * 1000.f);
        gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 2, str, r, g, b);
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
            ClientIndex = TraceEntity(viewOrigin, viewDir, lineLen, intersectPoint);

        cl_entity_t *pClient = gEngfuncs.GetEntityByIndex(ClientIndex);

        if (!CheckForClient(pClient))
            ClientIndex = 0;
    }

    std::string DebugMode_str = "Debug mode: ";
    int DebugModeWidth = gHUD.GetHudStringWidth(DebugMode_str.c_str());

    if (DebugMode > 0.0f) {
        sprintf(str, "Map: %s", gEngfuncs.pfnGetLevelName());
        gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 4, str, r, g, b);

        float clientTime = gEngfuncs.GetClientTime();
        formatTime(clientTime, str);
        gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 5, str, r, g, b);

        sprintf(str, "Name: %s", g_PlayerInfoList[localPlayer->index].name);
        gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 7, str, r, g, b);

        sprintf(str, "Index: %d", localPlayer->index);
        gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 8, str, r, g, b);

        ClientModelName(localPlayer, ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 9, r, g, b);

        gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 11, DebugMode_str.c_str(), r, g, b);
    }

    switch (DebugMode) {
        case 0:
            break;
        case 1:
            gHUD.DrawHudText(ScreenWidth / 1.5 + DebugModeWidth, gHUD.m_scrinfo.iCharHeight * 11, "1 (Minimal Info)", 0, 255, 0);
            break;
        case 2:
            gHUD.DrawHudText(ScreenWidth / 1.5 + DebugModeWidth, gHUD.m_scrinfo.iCharHeight * 11, "2 (Current Client Info)", 0, 255, 0);
            CurrentClientInfo(r, g, b);
            break;
        case 3:
            gHUD.DrawHudText(ScreenWidth / 1.5 + DebugModeWidth, gHUD.m_scrinfo.iCharHeight * 11, "3 (All Clients Info)", 0, 255, 0);
            if(!ClientIndex)
                gHUD.DrawHudTextCentered(ScreenWidth / 2, ScreenWidth / 3, "Aim at a player or follow them in 3rd person observer mode to gain information!", 255, 0, 0);
            else
                AllClientsInfo(r, g, b, ClientIndex);
            break;
        case 4:
            gHUD.DrawHudText(ScreenWidth / 1.5 + DebugModeWidth, gHUD.m_scrinfo.iCharHeight * 11, "4 (Entity Info) UNDONE", 255, 0, 0);
            break;
        default:
            gHUD.DrawHudText(ScreenWidth / 1.5 + DebugModeWidth, gHUD.m_scrinfo.iCharHeight * 11, "(UNKNOWN)", 255, 0, 0);
            break;
    }

    return 0;
}
