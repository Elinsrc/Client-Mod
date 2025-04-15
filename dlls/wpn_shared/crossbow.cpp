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
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "gamerules.h"

enum crossbow_e
{
	CROSSBOW_IDLE1 = 0,	// full
	CROSSBOW_IDLE2,		// empty
	CROSSBOW_FIDGET1,	// full
	CROSSBOW_FIDGET2,	// empty
	CROSSBOW_FIRE1,		// full
	CROSSBOW_FIRE2,		// reload
	CROSSBOW_FIRE3,		// empty
	CROSSBOW_RELOAD,	// from empty
	CROSSBOW_DRAW1,		// full
	CROSSBOW_DRAW2,		// empty
	CROSSBOW_HOLSTER1,	// full
	CROSSBOW_HOLSTER2	// empty
};

LINK_ENTITY_TO_CLASS( weapon_crossbow, CCrossbow )

void CCrossbow::Spawn()
{
	Precache();
	m_iId = WEAPON_CROSSBOW;
	SET_MODEL( ENT( pev ), "models/w_crossbow.mdl" );

	m_iDefaultAmmo = CROSSBOW_DEFAULT_GIVE;

	FallInit();// get ready to fall down.
}

int CCrossbow::AddToPlayer( CBasePlayer *pPlayer )
{
	if( CBasePlayerWeapon::AddToPlayer( pPlayer ) )
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev );
			WRITE_BYTE( m_iId );
		MESSAGE_END();
		return TRUE;
	}
	return FALSE;
}

void CCrossbow::Precache( void )
{
	PRECACHE_MODEL( "models/w_crossbow.mdl" );
	PRECACHE_MODEL( "models/v_crossbow.mdl" );
	PRECACHE_MODEL( "models/p_crossbow.mdl" );

	PRECACHE_SOUND( "weapons/xbow_fire1.wav" );
	PRECACHE_SOUND( "weapons/xbow_reload1.wav" );

	UTIL_PrecacheOther( "crossbow_bolt" );

	m_usCrossbow = PRECACHE_EVENT( 1, "events/crossbow1.sc" );
	m_usCrossbow2 = PRECACHE_EVENT( 1, "events/crossbow2.sc" );
}

int CCrossbow::GetItemInfo( ItemInfo *p )
{
	p->pszName = STRING( pev->classname );
	p->pszAmmo1 = "bolts";
	p->iMaxAmmo1 = BOLT_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = CROSSBOW_MAX_CLIP;
	p->iSlot = 2;
	p->iPosition = 2;
	p->iId = WEAPON_CROSSBOW;
	p->iFlags = 0;
	p->iWeight = CROSSBOW_WEIGHT;
	return 1;
}

BOOL CCrossbow::Deploy()
{
	if( m_iClip )
		return DefaultDeploy( "models/v_crossbow.mdl", "models/p_crossbow.mdl", CROSSBOW_DRAW1, "bow" );
	return DefaultDeploy( "models/v_crossbow.mdl", "models/p_crossbow.mdl", CROSSBOW_DRAW2, "bow" );
}

void CCrossbow::Holster( int skiplocal /* = 0 */ )
{
	m_fInReload = FALSE;// cancel any reload in progress.

	if( m_fInZoom )
	{
		SecondaryAttack();
	}

	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5f;
	if( m_iClip )
		SendWeaponAnim( CROSSBOW_HOLSTER1 );
	else
		SendWeaponAnim( CROSSBOW_HOLSTER2 );
}

void CCrossbow::PrimaryAttack( void )
{
	if( m_fInZoom && bIsMultiplayer() )
	{
		FireSniperBolt();
		return;
	}

	FireBolt();
}

// this function only gets called in multiplayer
void CCrossbow::FireSniperBolt()
{
	m_flNextPrimaryAttack = GetNextAttackDelay( 0.75f );

	if( m_iClip == 0 )
	{
		PlayEmptySound();
		return;
	}

	TraceResult tr;

	m_pPlayer->m_iWeaponVolume = QUIET_GUN_VOLUME;
	m_iClip--;

	int flags;

	flags = FEV_NOTHOST;

	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_usCrossbow2, 0.0f, g_vecZero, g_vecZero, 0, 0, m_iClip, 0, 0, 0 );

	// player "shoot" animation
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	Vector anglesAim = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;
	UTIL_MakeVectors( anglesAim );
	Vector vecSrc = m_pPlayer->GetGunPosition() - gpGlobals->v_up * 2.0f;
	Vector vecDir = gpGlobals->v_forward;

	UTIL_TraceLine( vecSrc, vecSrc + vecDir * 8192, dont_ignore_monsters, m_pPlayer->edict(), &tr );
}

void CCrossbow::FireBolt()
{
	TraceResult tr;

	if( m_iClip == 0 )
	{
		PlayEmptySound();
		return;
	}

	m_pPlayer->m_iWeaponVolume = QUIET_GUN_VOLUME;

	m_iClip--;

	int flags;

	flags = FEV_NOTHOST;

	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_usCrossbow, 0.0f, g_vecZero, g_vecZero, 0, 0, m_iClip, 0, 0, 0 );

	// player "shoot" animation
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	Vector anglesAim = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;
	UTIL_MakeVectors( anglesAim );

	anglesAim.x	= -anglesAim.x;

	if( !m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 )
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate( "!HEV_AMO0", FALSE, 0 );

	m_flNextPrimaryAttack = GetNextAttackDelay( 0.75f );

	m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.75f;

	if( m_iClip != 0 )
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 5.0f;
	else
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.75f;
}

void CCrossbow::SecondaryAttack()
{
	if( m_pPlayer->pev->fov != 0 )
	{
		m_pPlayer->pev->fov = m_pPlayer->m_iFOV = 0; // 0 means reset to default fov
		m_fInZoom = 0;
	}
	else if( m_pPlayer->pev->fov != 20 )
	{
		m_pPlayer->pev->fov = m_pPlayer->m_iFOV = 20;
		m_fInZoom = 1;
	}

	pev->nextthink = UTIL_WeaponTimeBase() + 0.1f;
	m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 1.0f;
}

void CCrossbow::Reload( void )
{
	if( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 || m_iClip == CROSSBOW_MAX_CLIP )
		return;

	if( m_pPlayer->pev->fov != 0 )
	{
		SecondaryAttack();
	}

	if( DefaultReload( CROSSBOW_MAX_CLIP, CROSSBOW_RELOAD, 4.5f ) )
	{
		EMIT_SOUND_DYN( ENT( m_pPlayer->pev ), CHAN_ITEM, "weapons/xbow_reload1.wav", RANDOM_FLOAT( 0.95f, 1.0f ), ATTN_NORM, 0, 93 + RANDOM_LONG( 0, 0xF ) );
	}
}

void CCrossbow::WeaponIdle( void )
{
	m_pPlayer->GetAutoaimVector( AUTOAIM_2DEGREES );  // get the autoaim vector but ignore it;  used for autoaim crosshair in DM

	ResetEmptySound();
	
	if( m_flTimeWeaponIdle < UTIL_WeaponTimeBase() )
	{
		float flRand = UTIL_SharedRandomFloat( m_pPlayer->random_seed, 0, 1 );
		if( flRand <= 0.75f )
		{
			if( m_iClip )
			{
				SendWeaponAnim( CROSSBOW_IDLE1 );
			}
			else
			{
				SendWeaponAnim( CROSSBOW_IDLE2 );
			}
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
		}
		else
		{
			if( m_iClip )
			{
				SendWeaponAnim( CROSSBOW_FIDGET1 );
				m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 90.0f / 30.0f;
			}
			else
			{
				SendWeaponAnim( CROSSBOW_FIDGET2 );
				m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 80.0f / 30.0f;
			}
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
		}
	}
}

class CCrossbowAmmo : public CBasePlayerAmmo
{
	void Spawn( void )
	{ 
		Precache();
		SET_MODEL( ENT( pev ), "models/w_crossbow_clip.mdl" );
		CBasePlayerAmmo::Spawn();
	}
	void Precache( void )
	{
		PRECACHE_MODEL( "models/w_crossbow_clip.mdl" );
		PRECACHE_SOUND( "items/9mmclip1.wav" );
	}
	BOOL AddAmmo( CBaseEntity *pOther )
	{ 
		if( pOther->GiveAmmo( AMMO_CROSSBOWCLIP_GIVE, "bolts", BOLT_MAX_CARRY ) != -1 )
		{
			EMIT_SOUND( ENT( pev ), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM );
			return TRUE;
		}
		return FALSE;
	}
};

LINK_ENTITY_TO_CLASS( ammo_crossbow, CCrossbowAmmo )
