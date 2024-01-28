#include <cmath>

#include "hud.h"
#include "cl_util.h"
#include "cl_entity.h"
#include "parsemsg.h"
#include "hud_debug.h"
#include "pm_defs.h"
#include "event_api.h"

// https://github.com/SNMetamorph/goldsrc-monitor

extern playermove_t *pmove;

int CHudDebug::Init()
{
    m_iFlags = HUD_ACTIVE;
    hud_debug = CVAR_CREATE("hud_debug", "0", FCVAR_ARCHIVE);
    gHUD.AddHudElem(this);
    return 0;
}

int CHudDebug::VidInit()
{
    return 1;
}

float CHudDebug::GetFrametime()
{
    float clientTime = gEngfuncs.GetClientTime();
    float timeDelta = clientTime - m_lastClientTime;

    m_frameTime += (timeDelta - m_frameTime);
    m_lastFrameTime = m_frameTime;
    m_lastClientTime = clientTime;
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

int CHudDebug::Draw(float flTime)
{
    if (hud_debug->value == 0.0f)
        return 0;

    int r, g, b;

    float timeDelta = GetFrametime();
    float fps_ = 1.f / timeDelta;

    char fps[256];
    sprintf(fps,"FPS: %.1f", fps_);

    char frametime[256];
    sprintf(frametime, "Frame Time: %.1f ms\n", timeDelta * 1000.f);

    if (fps_ > 89)
        UnpackRGB( r, g, b, RGB_GREENISH );
    else if (fps_ > 59)
        UnpackRGB( r, g, b, RGB_YELLOWISH );
    else
        UnpackRGB( r, g, b, RGB_REDISH );

    DrawString(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight, fps, r, g, b);
    DrawString(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 2, frametime, r, g, b);

    UnpackRGB(r, g, b, gHUD.m_iDefaultHUDColor);

    char pl_time[256];
    sprintf(pl_time, "Time: %.2f seconds\n", gEngfuncs.GetClientTime());
    DrawString(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 3, pl_time, r, g, b);

    cl_entity_t *localPlayer = gEngfuncs.GetLocalPlayer();

    float velocityNum = std::round(std::hypot(pmove->velocity[0], pmove->velocity[1]));

    char pl_velocity[30];
    sprintf(pl_velocity, "Velocity: %.2f u/s (%.2f, %.2f, %.2f)", velocityNum, pmove->velocity.x, pmove->velocity.y, pmove->velocity.z);
    DrawString(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 5, pl_velocity, r, g, b);

    char pl_origin[30];
    sprintf(pl_origin, "Origin: (%.2f, %.2f, %.2f)", localPlayer->origin.x, localPlayer->origin.y, localPlayer->origin.z);
    DrawString(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 6, pl_origin, r, g, b);

    char pl_angles[30];
    sprintf(pl_angles, "Anges: (%.2f, %.2f, %.2f)", localPlayer->angles.x, localPlayer->angles.y, localPlayer->angles.z);
    DrawString(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 7, pl_angles, r, g, b);

    char pl_movetype[30];
    sprintf(pl_movetype, "Movetype: %s", GetMovetypeName(localPlayer->curstate.movetype));
    DrawString(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 8, pl_movetype, r, g, b);

    vec3_t viewOffset;
    gEngfuncs.pEventAPI->EV_LocalPlayerViewheight(viewOffset);

    char pl_viewOffset[30];
    sprintf(pl_viewOffset, "View Offset: (%.2f, %.2f, %.2f)", viewOffset.x, viewOffset.y, viewOffset.z);
    DrawString(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 10, pl_viewOffset, r, g, b);

    const vec3_t &punchAngle = pmove->punchangle;

    char pl_punchAngle[30];
    sprintf(pl_punchAngle, "Punch Angle: (%.2f, %.2f, %.2f)", punchAngle.x, punchAngle.y, punchAngle.z);
    DrawString(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 11, pl_punchAngle, r, g, b);

    char pl_flags[30];
    sprintf(pl_flags, "Player Flags: %d", pmove->flags);
    DrawString(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 12, pl_flags, r, g, b);

    char pl_hulltype[30];
    sprintf(pl_hulltype, "Hull Type: %d", pmove->usehull);
    DrawString(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 13, pl_hulltype, r, g, b);

    char pl_gravity[30];
    sprintf(pl_gravity, "Gravity: %.2f", pmove->gravity);
    DrawString(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 14, pl_gravity, r, g, b);

    char pl_friction[30];
    sprintf(pl_friction, "Friction: %.2f", pmove->friction);
    DrawString(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 15, pl_friction, r, g, b);

    char pl_onground[30];
    sprintf(pl_onground, "On Ground: %s", pmove->onground ? "yes" : "no");
    DrawString(ScreenWidth / 1.5, gHUD.m_scrinfo.iCharHeight * 16, pl_onground, r, g, b);

    return 0;
}
