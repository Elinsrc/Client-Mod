//========= Copyright (c) 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef VOICE_STATUS_H
#define VOICE_STATUS_H
#pragma once

#if USE_VGUI
#include "VGUI_Label.h"
#include "VGUI_LineBorder.h"
#include "VGUI_ImagePanel.h"
#include "VGUI_BitmapTGA.h"
#include "VGUI_InputSignal.h"
#include "VGUI_Button.h"
#endif
#include "voice_common.h"
#include "cl_entity.h"
#include "voice_banmgr.h"
#if USE_VGUI
#include "vgui_checkbutton2.h"
#include "vgui_defaultinputsignal.h"
#endif

class CVoiceStatus;

#if USE_VGUI
class CVoiceLabel
{
public:
	vgui::Label			*m_pLabel;
	vgui::Label			*m_pBackground;
	vgui::ImagePanel	*m_pIcon;		// Voice icon next to player name.
	int					m_clientindex;	// Client index of the speaker. -1 if this label isn't being used.
};
#endif

// This is provided by each mod to access data that may not be the same across mods.
class IVoiceStatusHelper
{
public:
	virtual					~IVoiceStatusHelper()	{}

	// Get RGB color for voice status text about this player.
	virtual void			GetPlayerTextColor(int entindex, int color[3]) = 0;

#if USE_VGUI
	// Force it to update the cursor state.
	virtual void			UpdateCursorState() = 0;
#endif
	// Return the height above the bottom that the voice ack icons should be drawn at.
	virtual int				GetAckIconHeight() = 0;
#if USE_VGUI
	// Return true if the voice manager is allowed to show speaker labels
	// (mods usually return false when the scoreboard is up).
	virtual bool			CanShowSpeakerLabels() = 0;
#endif
};

#if USE_VGUI
//-----------------------------------------------------------------------------
// Purpose: Holds a color for the shared image
//-----------------------------------------------------------------------------
class VoiceImagePanel : public vgui::ImagePanel
{
	virtual void paintBackground()
	{
		if (_image!=null)
		{
			vgui::Color col;
			getFgColor(col);
			_image->setColor(col);
			_image->doPaint(this);
		}
	}
};
#endif

#if USE_VGUI
class CVoiceStatus : public CHudBase, public vgui::CDefaultInputSignal
#else
class CVoiceStatus : public CHudBase
#endif
{
public:
				CVoiceStatus();
	virtual		~CVoiceStatus();

// CHudBase overrides.
public:

	// Initialize the cl_dll's voice manager.
#if USE_VGUI
	virtual int Init( IVoiceStatusHelper *m_pHelper, vgui::Panel **pParentPanel);
#else
	virtual int Init( IVoiceStatusHelper *m_pHelper );
#endif
	
	// ackPosition is the bottom position of where CVoiceStatus will draw the voice acknowledgement labels.
	virtual int VidInit();


public:
	
	// Call from HUD_Frame each frame.
	void	Frame(double frametime);

	// Called when a player starts or stops talking.
	// entindex is -1 to represent the local client talking (before the data comes back from the server). 
	// When the server acknowledges that the local client is talking, then entindex will be gEngfuncs.GetLocalPlayer().
	// entindex is -2 to represent the local client's voice being acked by the server.
	void	UpdateSpeakerStatus(int entindex, qboolean bTalking);

#if USE_VGUI
	// sets the correct image in the label for the player
	void	UpdateSpeakerImage(vgui::Label *pLabel, int iPlayer);
#endif

	// Call from the HUD_CreateEntities function so it can add sprites above player heads.
	void	CreateEntities();

#if !USE_VGUI
	// Draw speaker icon without using vgui
	void	DrawNoVguiSpeakerIcon( int x, int y ,int playerIndex);

	int		Draw(float time);
#endif
	// Called when the server registers a change to who this client can hear.
	void	HandleVoiceMaskMsg(int iSize, void *pbuf);

	// The server sends this message initially to tell the client to send their state.
	void	HandleReqStateMsg(int iSize, void *pbuf);


// Squelch mode functions.
public:
#if USE_VGUI
	// When you enter squelch mode, pass in 
	void	StartSquelchMode();
	void	StopSquelchMode();
	bool	IsInSquelchMode();
#endif
	// returns true if the target client has been banned
	// playerIndex is of range 1..maxplayers
	bool	IsPlayerBlocked(int iPlayerIndex);

	// returns false if the player can't hear the other client due to game rules (eg. the other team)
	bool    IsPlayerAudible(int iPlayerIndex);

	bool	IsPlayerSpeaking(int iPlayerIndex);

	// blocks the target client from being heard
	void	SetPlayerBlockedState(int iPlayerIndex, bool blocked);

public:
#if USE_VGUI
	CVoiceLabel*	FindVoiceLabel(int clientindex);	// Find a CVoiceLabel representing the specified speaker. 
													// Returns NULL if none.
													// entindex can be -1 if you want a currently-unused voice label.
	CVoiceLabel*	GetFreeVoiceLabel();			// Get an unused voice label. Returns NULL if none.

	void			RepositionLabels();

	void			FreeBitmaps();
#endif

	void			UpdateServerState(bool bForce);

#if USE_VGUI
	// Update the button artwork to reflect the client's current state.
	void			UpdateBanButton(int iClient);
#endif


public:

	enum			{MAX_VOICE_SPEAKERS=7};

	float			m_LastUpdateServerState;		// Last time we called this function.
	int				m_bServerModEnable;				// What we've sent to the server about our "voice_modenable" cvar.

#if USE_VGUI
	vgui::Panel		**m_pParentPanel;
#endif

	CPlayerBitVec	m_VoicePlayers;		// Who is currently talking. Indexed by client index.
	
	// This is the gamerules-defined list of players that you can hear. It is based on what teams people are on 
	// and is totally separate from the ban list. Indexed by client index.
	CPlayerBitVec	m_AudiblePlayers;

	// Players who have spoken at least once in the game so far
	CPlayerBitVec	m_VoiceEnabledPlayers;	

	// This is who the server THINKS we have banned (it can become incorrect when a new player arrives on the server).
	// It is checked periodically, and the server is told to squelch or unsquelch the appropriate players.
	CPlayerBitVec	m_ServerBannedPlayers;

	cl_entity_s		m_VoiceHeadModels[VOICE_MAX_PLAYERS];			// These aren't necessarily in the order of players. They are just
																	// a place for it to put data in during CreateEntities.

#if USE_VGUI
	IVoiceStatusHelper	*m_pHelper;		// Each mod provides an implementation of this.

	
	// Scoreboard icons.
	double			m_BlinkTimer;			// Blink scoreboard icons..
	vgui::BitmapTGA	*m_pScoreboardNeverSpoken;
	vgui::BitmapTGA	*m_pScoreboardNotSpeaking;
	vgui::BitmapTGA	*m_pScoreboardSpeaking;
	vgui::BitmapTGA	*m_pScoreboardSpeaking2;
	vgui::BitmapTGA	*m_pScoreboardSquelch;
	vgui::BitmapTGA	*m_pScoreboardBanned;
	
	vgui::Label		   *m_pBanButtons[VOICE_MAX_PLAYERS];		// scoreboard buttons.

	// Squelch mode stuff.
	bool				m_bInSquelchMode;
#endif

	HSPRITE				m_VoiceHeadModel;		// Voice head model (goes above players who are speaking).
	float				m_VoiceHeadModelHeight;	// Height above their head to place the model.

#if USE_VGUI
	vgui::Image			*m_pSpeakerLabelIcon;	// Icon next to speaker labels.

	// Lower-right icons telling when the local player is talking..
	vgui::BitmapTGA		*m_pLocalBitmap;		// Represents the local client talking.
	vgui::BitmapTGA		*m_pAckBitmap;			// Represents the server ack'ing the client talking.
	vgui::ImagePanel	*m_pLocalLabel;			// Represents the local client talking.
#endif

	bool				m_bTalking;				// Set to true when the client thinks it's talking.
	bool				m_bServerAcked;			// Set to true when the server knows the client is talking.

public:
	
	CVoiceBanMgr		m_BanMgr;				// Tracks which users we have squelched and don't want to hear.

public:

	bool				m_bBanMgrInitialized;

#if USE_VGUI
	// Labels telling who is speaking.
	CVoiceLabel			m_Labels[MAX_VOICE_SPEAKERS];
#endif

	// Cache the game directory for use when we shut down
	char *				m_pchGameDir;
};


// Get the (global) voice manager. 
CVoiceStatus* GetClientVoiceMgr();


#endif // VOICE_STATUS_H
