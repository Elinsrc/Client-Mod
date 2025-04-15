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
#include "effects.h"
#include "gamerules.h"

#define	TRIPMINE_PRIMARY_VOLUME		450

enum tripmine_e
{
	TRIPMINE_IDLE1 = 0,
	TRIPMINE_IDLE2,
	TRIPMINE_ARM1,
	TRIPMINE_ARM2,
	TRIPMINE_FIDGET,
	TRIPMINE_HOLSTER,
	TRIPMINE_DRAW,
	TRIPMINE_WORLD,
	TRIPMINE_GROUND
};

LINK_ENTITY_TO_CLASS( weapon_tripmine, CTripmine )

void CTripmine::Spawn()
{
	Precache();
	m_iId = WEAPON_TRIPMINE;
	SET_MODEL( ENT( pev ), "models/v_tripmine.mdl" );
	pev->frame = 0;

	pev->body = 0;

	pev->sequence = TRIPMINE_GROUND;
	// ResetSequenceInfo();
	pev->framerate = 0;

	FallInit();// get ready to fall down

	m_iDefaultAmmo = TRIPMINE_DEFAULT_GIVE;

	if( !bIsMultiplayer() )
	{
		UTIL_SetSize( pev, Vector( -16.0f, -16.0f, 0.0f ), Vector( 16.0f, 16.0f, 28.0f ) ); 
	}
}

void CTripmine::Precache( void )
{
	PRECACHE_MODEL( "models/v_tripmine.mdl" );
	PRECACHE_MODEL( "models/p_tripmine.mdl" );
	UTIL_PrecacheOther( "monster_tripmine" );

	m_usTripFire = PRECACHE_EVENT( 1, "events/tripfire.sc" );
}

int CTripmine::GetItemInfo( ItemInfo *p )
{
	p->pszName = STRING( pev->classname );
	p->pszAmmo1 = "Trip Mine";
	p->iMaxAmmo1 = TRIPMINE_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 4;
	p->iPosition = 2;
	p->iId = m_iId = WEAPON_TRIPMINE;
	p->iWeight = TRIPMINE_WEIGHT;
	p->iFlags = ITEM_FLAG_LIMITINWORLD | ITEM_FLAG_EXHAUSTIBLE;

	return 1;
}

BOOL CTripmine::Deploy()
{
	pev->body = 0;
	return DefaultDeploy( "models/v_tripmine.mdl", "models/p_tripmine.mdl", TRIPMINE_DRAW, "trip" );
}

void CTripmine::Holster( int skiplocal /* = 0 */ )
{
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5f;

	if( !m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] )
	{
		// out of mines
		m_pPlayer->pev->weapons &= ~( 1 << WEAPON_TRIPMINE );
		DestroyItem();
	}

	SendWeaponAnim( TRIPMINE_HOLSTER );
	EMIT_SOUND( ENT( m_pPlayer->pev ), CHAN_WEAPON, "common/null.wav", 1.0f, ATTN_NORM );
}

void CTripmine::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 )
		return;

	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );
	Vector vecSrc = m_pPlayer->GetGunPosition();
	Vector vecAiming = gpGlobals->v_forward;

	TraceResult tr;

	UTIL_TraceLine( vecSrc, vecSrc + vecAiming * 128.0f, dont_ignore_monsters, ENT( m_pPlayer->pev ), &tr );

	int flags;

	flags = FEV_NOTHOST;

	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_usTripFire, 0.0f, g_vecZero, g_vecZero, 0.0f, 0.0f, 0, 0, m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] == 1, 0 );

	if( tr.flFraction < 1.0f )
	{
		CBaseEntity *pEntity = CBaseEntity::Instance( tr.pHit );
		if( pEntity && !( pEntity->pev->flags & FL_CONVEYOR ) )
		{
			Vector angles = UTIL_VecToAngles( tr.vecPlaneNormal );

			CBaseEntity::Create( "monster_tripmine", tr.vecEndPos + tr.vecPlaneNormal * 8.0f, angles, m_pPlayer->edict() );

			m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;

			// player "shoot" animation
			m_pPlayer->SetAnimation( PLAYER_ATTACK1 );
			
			if( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 )
			{
				// no more mines! 
				RetireWeapon();
				return;
			}
		}
		/*else
		{
			// ALERT( at_console, "no deploy\n" );
		}*/
	}
	/*else
	{

	}*/

	m_flNextPrimaryAttack = GetNextAttackDelay( 0.3 );
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}

void CTripmine::WeaponIdle( void )
{
	pev->body = 0;

	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] > 0 )
	{
		SendWeaponAnim( TRIPMINE_DRAW );
	}
	else
	{
		RetireWeapon(); 
		return;
	}

	int iAnim;
	float flRand = UTIL_SharedRandomFloat( m_pPlayer->random_seed, 0, 1 );
	if( flRand <= 0.25f )
	{
		iAnim = TRIPMINE_IDLE1;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 90.0f / 30.0f;
	}
	else if( flRand <= 0.75f )
	{
		iAnim = TRIPMINE_IDLE2;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 60.0f / 30.0f;
	}
	else
	{
		iAnim = TRIPMINE_FIDGET;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 100.0f / 30.0f;
	}

	SendWeaponAnim( iAnim );
}
