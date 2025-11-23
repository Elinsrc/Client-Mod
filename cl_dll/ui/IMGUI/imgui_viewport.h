#ifndef IMGUI_VIEWPORT_H
#define IMGUI_VIEWPORT_H

#include "hud.h"
#include "imgui_ScorePanel.h"
#include "imgui_MOTD.h"

extern int iTeamColors[5][3];
extern int iNumberOfTeamColors;

class CImGuiViewport
{
public:
    static CImGuiViewport &GetInstance();

    CImGuiViewport();
    ~CImGuiViewport();

    void Initialize();
    void ShowScoreBoard();
    void HideScoreBoard();
    void GetAllPlayersInfo();

    int MsgFunc_ValClass(const char *pszName, int iSize, void *pbuf );
    int MsgFunc_TeamNames(const char *pszName, int iSize, void *pbuf );
    int MsgFunc_Feign(const char *pszName, int iSize, void *pbuf );
    int MsgFunc_Detpack(const char *pszName, int iSize, void *pbuf );
    int MsgFunc_IMGUIMenu(const char *pszName, int iSize, void *pbuf );
    int MsgFunc_MOTD( const char *pszName, int iSize, void *pbuf );
    int MsgFunc_BuildSt( const char *pszName, int iSize, void *pbuf );
    int MsgFunc_RandomPC( const char *pszName, int iSize, void *pbuf );
    int MsgFunc_ServerName( const char *pszName, int iSize, void *pbuf );
    int MsgFunc_ScoreInfo( const char *pszName, int iSize, void *pbuf );
    int MsgFunc_TeamScore( const char *pszName, int iSize, void *pbuf );
    int MsgFunc_TeamInfo( const char *pszName, int iSize, void *pbuf );
    int MsgFunc_Spectator( const char *pszName, int iSize, void *pbuf );
    int MsgFunc_AllowSpec( const char *pszName, int iSize, void *pbuf );
    int MsgFunc_SpecFade( const char *pszName, int iSize, void *pbuf );
    int MsgFunc_ResetFade( const char *pszName, int iSize, void *pbuf );

    void DeathMsg( int killer, int victim );

public:
    char m_szMOTD[ MAX_MOTD_LENGTH ];
    char m_szServerName[ MAX_SERVERNAME_LENGTH ];

private:
    int m_iAllowSpectators;
    int m_iGotAllMOTD;
};

extern CImGuiViewport &g_ImGuiViewport;
#endif // IMGUI_VIEWPORT_H
