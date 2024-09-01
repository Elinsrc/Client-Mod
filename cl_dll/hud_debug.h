#pragma once
#include <cstdint>

class CHudDebug : public CHudBase
{
    cvar_t *cl_debug;
    cvar_t *cl_debug_showfps;

    float m_frameTime;
    float m_lastFrameTime;
    float m_lastSysTime;

public:
    virtual int Init();
    virtual int VidInit();
    virtual float GetFrametime();
    virtual const char *GetMovetypeName(int moveType);
    virtual void formatTime(float totalSeconds, char* output);
    virtual int Draw(float flTime);
};
