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
#include "soundent.h"
#include "gamerules.h"

enum w_squeak_e
{
	WSQUEAK_IDLE1 = 0,
	WSQUEAK_FIDGET,
	WSQUEAK_JUMP,
	WSQUEAK_RUN
};

enum squeak_e
{
	SQUEAK_IDLE1 = 0,
	SQUEAK_FIDGETFIT,
	SQUEAK_FIDGETNIP,
	SQUEAK_DOWN,
	SQUEAK_UP,
	SQUEAK_THROW
};

LINK_ENTITY_TO_CLASS( weapon_snark, CSqueak )

void CSqueak::Spawn()
{
	Precache();
	m_iId = WEAPON_SNARK;
	SET_MODEL( ENT( pev ), "models/w_sqknest.mdl" );

	FallInit();//get ready to fall down.

	m_iDefaultAmmo = SNARK_DEFAULT_GIVE;

	pev->sequence = 1;
	pev->animtime = gpGlobals->time;
	pev->framerate = 1.0f;
}

void CSqueak::Precache( void )
{
	PRECACHE_MODEL( "models/w_sqknest.mdl" );
	PRECACHE_MODEL( "models/v_squeak.mdl" );
	PRECACHE_MODEL( "models/p_squeak.mdl" );
	PRECACHE_SOUND( "squeek/sqk_hunt2.wav" );
	PRECACHE_SOUND( "squeek/sqk_hunt3.wav" );
	UTIL_PrecacheOther( "monster_snark" );

	m_usSnarkFire = PRECACHE_EVENT( 1, "events/snarkfire.sc" );
}

int CSqueak::GetItemInfo( ItemInfo *p )
{
	p->pszName = STRING( pev->classname );
	p->pszAmmo1 = "Snarks";
	p->iMaxAmmo1 = SNARK_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 4;
	p->iPosition = 3;
	p->iId = m_iId = WEAPON_SNARK;
	p->iWeight = SNARK_WEIGHT;
	p->iFlags = ITEM_FLAG_LIMITINWORLD | ITEM_FLAG_EXHAUSTIBLE;

	return 1;
}

BOOL CSqueak::Deploy()
{
	// play hunt sound
	float flRndSound = RANDOM_FLOAT( 0.0f, 1.0f );

	if( flRndSound <= 0.5f )
		EMIT_SOUND_DYN( ENT( pev ), CHAN_VOICE, "squeek/sqk_hunt2.wav", 1, ATTN_NORM, 0, 100 );
	else
		EMIT_SOUND_DYN( ENT( pev ), CHAN_VOICE, "squeek/sqk_hunt3.wav", 1, ATTN_NORM, 0, 100 );

	m_pPlayer->m_iWeaponVolume = QUIET_GUN_VOLUME;

	const BOOL result = DefaultDeploy( "models/v_squeak.mdl", "models/p_squeak.mdl", SQUEAK_UP, "squeak" );

	return result;
}

void CSqueak::Holster( int skiplocal /* = 0 */ )
{
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5f;

	if( !m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] )
	{
		m_pPlayer->pev->weapons &= ~( 1 << WEAPON_SNARK );
		DestroyItem();
		return;
	}

	SendWeaponAnim( SQUEAK_DOWN );
	EMIT_SOUND( ENT( m_pPlayer->pev ), CHAN_WEAPON, "common/null.wav", 1.0f, ATTN_NORM );
}

void CSqueak::PrimaryAttack()
{
	if( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] )
	{
		UTIL_MakeVectors( m_pPlayer->pev->v_angle );
		TraceResult tr;
		Vector trace_origin;

		// HACK HACK:  Ugly hacks to handle change in origin based on new physics code for players
		// Move origin up if crouched and start trace a bit outside of body ( 20 units instead of 16 )
		trace_origin = m_pPlayer->pev->origin;
		if( m_pPlayer->pev->flags & FL_DUCKING )
		{
			trace_origin = trace_origin - ( VEC_HULL_MIN - VEC_DUCK_HULL_MIN );
		}

		// find place to toss monster
		UTIL_TraceLine( trace_origin + gpGlobals->v_forward * 20.0f, trace_origin + gpGlobals->v_forward * 64.0f, dont_ignore_monsters, NULL, &tr );

		int flags;

		flags = FEV_NOTHOST;

		PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_usSnarkFire, 0.0f, g_vecZero, g_vecZero, 0.0f, 0.0f, 0, 0, 0, 0 );

		if( tr.fAllSolid == 0 && tr.fStartSolid == 0 && tr.flFraction > 0.25f )
		{
			// player "shoot" animation
			m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

			// play hunt sound
			float flRndSound = RANDOM_FLOAT( 0.0f, 1.0f );

			if( flRndSound <= 0.5f )
				EMIT_SOUND_DYN( ENT( pev ), CHAN_VOICE, "squeek/sqk_hunt2.wav", 1, ATTN_NORM, 0, 105 );
			else 
				EMIT_SOUND_DYN( ENT( pev ), CHAN_VOICE, "squeek/sqk_hunt3.wav", 1, ATTN_NORM, 0, 105 );

			m_pPlayer->m_iWeaponVolume = QUIET_GUN_VOLUME;

			m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;

			m_fJustThrown = 1;

			m_flNextPrimaryAttack = GetNextAttackDelay( 0.3f );
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.0f;
		}
	}
}

void CSqueak::SecondaryAttack( void )
{
}

void CSqueak::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_fJustThrown )
	{
		m_fJustThrown = 0;

		if( !m_pPlayer->m_rgAmmo[PrimaryAmmoIndex()] )
		{
			RetireWeapon();
			return;
		}

		SendWeaponAnim( SQUEAK_UP );
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
		return;
	}

	int iAnim;
	float flRand = UTIL_SharedRandomFloat( m_pPlayer->random_seed, 0, 1 );
	if( flRand <= 0.75f )
	{
		iAnim = SQUEAK_IDLE1;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 30.0f / 16.0f * 2.0f;
	}
	else if( flRand <= 0.875f )
	{
		iAnim = SQUEAK_FIDGETFIT;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 70.0f / 16.0f;
	}
	else
	{
		iAnim = SQUEAK_FIDGETNIP;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 80.0f / 16.0f;
	}
	SendWeaponAnim( iAnim );
}
