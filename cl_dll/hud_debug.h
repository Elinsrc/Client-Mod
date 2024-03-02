#pragma once
#include <cstdint>

class CHudDebug : public CHudBase
{
    cvar_t *hud_debug;

    float m_frameTime;
    float m_lastFrameTime;
    float m_lastSysTime;

public:
    virtual int Init();
    virtual int VidInit();
    virtual float GetFrametime();
    virtual const char *GetMovetypeName(int moveType);
    virtual int Draw(float flTime);
};
