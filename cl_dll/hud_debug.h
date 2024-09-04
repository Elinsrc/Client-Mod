#pragma once
#include <cstdint>
#include "pmtrace.h"

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
    virtual const char *GetRenderModeName(int renderMode);
    virtual const char *GetRenderFxName(int renderFx);
    virtual void formatTime(float totalSeconds, char* output);
    virtual void Box( int x, int y, int w, int h, int linewidth, int r, int g, int b, int a );
	virtual void BoxOutline(float x, float y, float w, float h, float linewidth, int r, int g, int b, int a);
    virtual void BoxCorner(int x, int y, int w, int h, int linewidth, int r, int g, int b, int a);
    virtual void BoxCornerOutline(int x, int y, int w, int h, int linewidth, int r, int g, int b, int a);
    virtual bool CalcScreen(float *Origin, float *VecScreen);
    virtual void ClientModelName(cl_entity_s *entity, int x, int y, int r, int g, int b);
    virtual void CurrentClientInfo( int r, int g, int b);
    virtual bool CheckForClient(cl_entity_s *pEnt);
    virtual vec3_t GetEntityVelocityApprox(cl_entity_t *entity, int approxStep );
    virtual void TraceLine(vec3_t &origin, vec3_t &dir, float lineLen, pmtrace_t *traceData);
    virtual int TraceEntity(vec3_t origin, vec3_t dir, float distance, vec3_t &intersect);
    virtual void AllClientsInfo(int r, int g, int b);
    virtual int Draw(float flTime);
};
