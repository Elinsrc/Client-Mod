//++ BulliT
#pragma once

class CHudLongjump: public CHudBase
{
public:
    int Init( void );
    int VidInit( void );
    int Draw(float flTime);
    void Reset(void);
    int MsgFunc_Longjump(const char *pszName, int iSize, void *pbuf);

private:
    float m_flTurnoff;
};

//-- Martin Webrant
