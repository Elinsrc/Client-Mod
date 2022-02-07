/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
// battery.cpp
//
// implementation of CHudBattery class
//

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"

#include <string.h>
#include <stdio.h>

cvar_t *hud_battery100; 
//cvar_t *hud_battery80;
cvar_t *hud_battery60;
cvar_t *hud_battery40;
cvar_t *hud_battery20;

DECLARE_MESSAGE( m_Battery, Battery )

int CHudBattery::Init( void )
{
	m_iBat = 0;
	m_fFade = 0;
	m_iFlags = 0;

	HOOK_MESSAGE( Battery );
	
	hud_battery100 = CVAR_CREATE("hud_battery100", "0 250 0", FCVAR_ARCHIVE);
	//hud_battery80 = CVAR_CREATE("hud_battery80", "0 0 0", FCVAR_ARCHIVE)
	hud_battery60 = CVAR_CREATE("hud_battery60", "250 250 0", FCVAR_ARCHIVE);
	hud_battery40 = CVAR_CREATE("hud_battery40", "250 100 0", FCVAR_ARCHIVE);
	hud_battery20 = CVAR_CREATE("hud_battery20", "250 0 0", FCVAR_ARCHIVE);

	gHUD.AddHudElem( this );

	return 1;
}

int CHudBattery::VidInit( void )
{
	int HUD_suit_empty = gHUD.GetSpriteIndex( "suit_empty" );
	int HUD_suit_full = gHUD.GetSpriteIndex( "suit_full" );

	m_hSprite1 = m_hSprite2 = 0;  // delaying get sprite handles until we know the sprites are loaded
	m_prc1 = &gHUD.GetSpriteRect( HUD_suit_empty );
	m_prc2 = &gHUD.GetSpriteRect( HUD_suit_full );
	m_iHeight = m_prc2->bottom - m_prc1->top;
	m_fFade = 0;
	return 1;
}

int CHudBattery::MsgFunc_Battery( const char *pszName,  int iSize, void *pbuf )
{
	m_iFlags |= HUD_ACTIVE;

	BEGIN_READ( pbuf, iSize );
	int x = READ_SHORT();

	if( x != m_iBat )
	{
		m_fFade = FADE_TIME;
		m_iBat = x;
	}

	return 1;
}

void CHudBattery::GetPainColor( int &r, int &g, int &b )
{
  if ( CVAR_GET_FLOAT("hud_new") )
  {
    int iBattery = m_iBat;
    
    if( m_iBat > 59 )
    {
      const char *color = hud_battery100->string;
      sscanf( color, "%d %d %d", &r, &g, &b);
    }
    else if( m_iBat > 39 )
    {
      const char *color = hud_battery60->string;
      sscanf( color, "%d %d %d", &r, &g, &b); 
    }
    else if( m_iBat > 19 )
    {
      const char *color = hud_battery40->string;
      sscanf( color, "%d %d %d", &r, &g, &b);
    }
    else if( m_iBat > -1 )
    {
      const char *color = hud_battery20->string;
      sscanf( color, "%d %d %d", &r, &g, &b);
    }
  }
  else
    UnpackRGB( r, g, b, RGB_YELLOWISH );
}

int CHudBattery::Draw( float flTime )
{
	if( gHUD.m_iHideHUDDisplay & HIDEHUD_HEALTH )
		return 1;

	int r, g, b, x, y, a;
	wrect_t rc;

	rc = *m_prc2;
	rc.top  += m_iHeight * ( (float)( 100 - ( Q_min( 100, m_iBat ) ) ) * 0.01f );	// battery can go from 0 to 100 so * 0.01 goes from 0 to 1

	//UnpackRGB( r, g, b, RGB_YELLOWISH );

	if( !( gHUD.m_iWeaponBits & ( 1 << ( WEAPON_SUIT ) ) ) )
		return 1;

	// Has health changed? Flash the health #
	if( m_fFade )
	{
		if( m_fFade > FADE_TIME )
			m_fFade = FADE_TIME;

		m_fFade -= ( (float)gHUD.m_flTimeDelta * 20.0f );
		if( m_fFade <= 0 )
		{
			a = 128;
			m_fFade = 0;
		}

		// Fade the health number back to dim
		a = MIN_ALPHA + ( m_fFade / FADE_TIME ) * 128;
	}
	else
		a = MIN_ALPHA;
		
	GetPainColor( r, g, b );
	ScaleColors( r, g, b, a );

	int iOffset = ( m_prc1->bottom - m_prc1->top ) / 6;

	y = ScreenHeight - gHUD.m_iFontHeight - gHUD.m_iFontHeight / 2;
	x = ScreenWidth / 5;

	// make sure we have the right sprite handles
	if( !m_hSprite1 )
		m_hSprite1 = gHUD.GetSprite( gHUD.GetSpriteIndex( "suit_empty" ) );
	if( !m_hSprite2 )
		m_hSprite2 = gHUD.GetSprite( gHUD.GetSpriteIndex( "suit_full" ) );

	SPR_Set( m_hSprite1, r, g, b );
	SPR_DrawAdditive( 0,  x, y - iOffset, m_prc1 );

	if( rc.bottom > rc.top )
	{
		SPR_Set( m_hSprite2, r, g, b );
		SPR_DrawAdditive( 0, x, y - iOffset + ( rc.top - m_prc2->top ), &rc );
	}

	x += ( m_prc1->right - m_prc1->left );
	x = gHUD.DrawHudNumber( x, y, DHN_3DIGITS | DHN_DRAWZERO, m_iBat, r, g, b );

	return 1;
}
