#include <cmath>

#include "hud.h"
#include "cl_util.h"
#include "cl_entity.h"
#include "parsemsg.h"
#include "hud_debug.h"
#include "pm_defs.h"
#include "event_api.h"

#ifdef _WIN32
#define HSPRITE DeletedWinapi_HSPRITE// prevent windows.h from defining it
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
    static LARGE_INTEGER	perfFreq;
    static LARGE_INTEGER	clockStart;
    LARGE_INTEGER		    currentTime;
    LONGLONG                timeDiff;

    if (!perfFreq.QuadPart)
    {
        QueryPerformanceFrequency(&perfFreq);
        QueryPerformanceCounter(&clockStart);
    }

    QueryPerformanceCounter(&currentTime);
    timeDiff = currentTime.QuadPart - clockStart.QuadPart;
    return (float)timeDiff / (float)perfFreq.QuadPart;
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec / 1000000000.0;
#endif
}

float CHudDebug::GetFrametime()
{
    const float smoothFactor    = 0.24f;
    const float diffThreshold   = 0.13f;
    float currSysTime           = GetCurrentSysTime();
    float timeDelta             = currSysTime - m_lastSysTime;

    if ((timeDelta - m_lastFrameTime) > diffThreshold)
        timeDelta = m_lastFrameTime;

    m_frameTime       += (timeDelta - m_frameTime) * smoothFactor;
    m_lastFrameTime   = m_frameTime;
    m_lastSysTime     = currSysTime;
    return m_frameTime;
}

const char *CHudDebug::GetMovetypeName(int moveType)
{
    switch (moveType)
    {
        case MOVETYPE_BOUNCE:           return "Bounce";
        case MOVETYPE_BOUNCEMISSILE:    return "Bounce-missle";
        case MOVETYPE_FLY:              return "Fly";
        case MOVETYPE_FLYMISSILE:       return "Fly-missle";
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

static const unsigned char colors[4][3] =
{
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
    int seconds = (int)totalSeconds;
    int days = seconds / (24 * 3600);
    seconds %= (24 * 3600);
    int hours = seconds / 3600;
    seconds %= 3600;
    int minutes = seconds / 60;
    seconds %= 60;

    char buffer[256] = "";
    if (days > 0) {
        sprintf(buffer + strlen(buffer), "%dd ", days);
    }
    if (hours > 0) {
        sprintf(buffer + strlen(buffer), "%dh ", hours);
    }
    if (minutes > 0) {
        sprintf(buffer + strlen(buffer), "%dm ", minutes);
    }
    sprintf(buffer + strlen(buffer), "%ds", seconds);

    sprintf(output, "Map Time: %s", buffer);
}

extern struct Color {
    int r;
    int g;
    int b;
} top, bottom;

int CHudDebug::Draw(float flTime)
{
    float timeDelta = GetFrametime();
    float fps = 1.f / timeDelta;
    char str[256];

    const unsigned char* color = getColorForFPS(fps);
    int r = color[0];
    int g = color[1];
    int b = color[2];

    if (cl_debug->value > 0.0f || cl_debug_showfps->value > 0.0f)
    {
        sprintf(str,"FPS: %.1f", fps);
        gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight, str, r, g, b);

        sprintf(str, "Frame Time: %.1f ms\n", timeDelta * 1000.f);
        gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 2, str, r, g, b);
    }

    if (cl_debug->value == 0.0f)
        return 0;

    r = colors[3][0];
    g = colors[3][1];
    b = colors[3][2];

    sprintf(str, "Map: %s", gEngfuncs.pfnGetLevelName());
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 4, str, r, g, b);

    float clientTime = gEngfuncs.GetClientTime();
    formatTime(clientTime, str);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 5, str, r, g, b);

    cl_entity_t *localPlayer = gEngfuncs.GetLocalPlayer();

    hud_player_info_t pl_info;
    memset( &pl_info, 0, sizeof( pl_info ) );
    gEngfuncs.pfnGetPlayerInfo( localPlayer->index, &pl_info );

    sprintf(str, "Name: %s", pl_info.name);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 7, str, r, g, b);

    std::string firstcolor;
    std::string secondcolor;
    std::string modelname = pl_info.model;
    std::string model_str = "Model:";

    size_t mid = modelname.length() / 2;
    firstcolor = modelname.substr(0, mid);
    secondcolor = modelname.substr(mid);

    gHUD.HUEtoRGB(pl_info.topcolor, top.r, top.g, top.b);
    gHUD.HUEtoRGB(pl_info.bottomcolor, bottom.r, bottom.g, bottom.b);

    int firstcolorWidth = gHUD.GetHudStringWidth(firstcolor.c_str());
    int ModelWidth = gHUD.GetHudStringWidth(model_str.c_str());

    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 8, model_str.c_str(), r, g, b);
    gHUD.DrawHudText(ScreenWidth / 1.5 + ModelWidth, gHUD.m_scrinfo.iCharHeight * 8, firstcolor.c_str(), top.r, top.g, top.b );
    gHUD.DrawHudText(ScreenWidth / 1.5 + ModelWidth + firstcolorWidth, gHUD.m_scrinfo.iCharHeight * 8, secondcolor.c_str(), bottom.r, bottom.g, bottom.b );


    float velocityNum = std::round(std::hypot(pmove->velocity[0], pmove->velocity[1]));

    sprintf(str, "Velocity: %.2f u/s (%.2f, %.2f, %.2f)", velocityNum, pmove->velocity.x, pmove->velocity.y, pmove->velocity.z);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 10, str, r, g, b);

    sprintf(str, "Origin: (%.2f, %.2f, %.2f)", localPlayer->origin.x, localPlayer->origin.y, localPlayer->origin.z);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 11, str, r, g, b);

    sprintf(str, "Anges: (%.2f, %.2f, %.2f)", localPlayer->angles.x, localPlayer->angles.y, localPlayer->angles.z);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 12, str, r, g, b);

    sprintf(str, "Movetype: %s", GetMovetypeName(localPlayer->curstate.movetype));
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 13, str, r, g, b);

    vec3_t viewOffset;
    gEngfuncs.pEventAPI->EV_LocalPlayerViewheight(viewOffset);

    sprintf(str, "View Offset: (%.2f, %.2f, %.2f)", viewOffset.x, viewOffset.y, viewOffset.z);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 15, str, r, g, b);

    const vec3_t &punchAngle = pmove->punchangle;

    sprintf(str, "Punch Angle: (%.2f, %.2f, %.2f)", punchAngle.x, punchAngle.y, punchAngle.z);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 16, str, r, g, b);

    sprintf(str, "Player Flags: %d", pmove->flags);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 17, str, r, g, b);

    sprintf(str, "Hull Type: %d", pmove->usehull);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 18, str, r, g, b);

    sprintf(str, "Gravity: %.2f", pmove->gravity);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 19, str, r, g, b);

    sprintf(str, "Friction: %.2f", pmove->friction);
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 20, str, r, g, b);

    sprintf(str, "On Ground: %s", pmove->onground ? "yes" : "no");
    gHUD.DrawHudText(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 21, str, r, g, b);

    return 0;
}
