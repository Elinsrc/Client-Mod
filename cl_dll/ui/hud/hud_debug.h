#pragma once
#include "custom_utils.h"

class CHudDebug : public CHudBase
{
    cvar_t *cl_debug;
    cvar_t *cl_debug_showfps;

    CustomUtils m_CustomUtils;

public:
    int Init();
    int VidInit();
    void ClientModelName(cl_entity_s *entity, int x, int y, int r, int g, int b);
    void CurrentClientInfo( int r, int g, int b);
    void AllClientsInfo(int r, int g, int b, int ClientIndex);
    int Draw(float flTime);
};
