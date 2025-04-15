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
#include "shake.h"
#include "gamerules.h"
#include "game.h"

#define	GAUSS_PRIMARY_CHARGE_VOLUME	256// how loud gauss is while charging
#define GAUSS_PRIMARY_FIRE_VOLUME	450// how loud gauss is when discharged

enum gauss_e
{
	GAUSS_IDLE = 0,
	GAUSS_IDLE2,
	GAUSS_FIDGET,
	GAUSS_SPINUP,
	GAUSS_SPIN,
	GAUSS_FIRE,
	GAUSS_FIRE2,
	GAUSS_HOLSTER,
	GAUSS_DRAW
};

LINK_ENTITY_TO_CLASS( weapon_gauss, CGauss )

float CGauss::GetFullChargeTime( void )
{
	if( bIsMultiplayer() )
	{
		return 1.5f;
	}

	return 4.0f;
}

extern int g_irunninggausspred;

void CGauss::Spawn()
{
	Precache();
	m_iId = WEAPON_GAUSS;
	SET_MODEL( ENT( pev ), "models/w_gauss.mdl" );

	m_iDefaultAmmo = GAUSS_DEFAULT_GIVE;

	FallInit();// get ready to fall down.
}

void CGauss::Precache( void )
{
	PRECACHE_MODEL( "models/w_gauss.mdl" );
	PRECACHE_MODEL( "models/v_gauss.mdl" );
	PRECACHE_MODEL( "models/p_gauss.mdl" );

	PRECACHE_SOUND( "items/9mmclip1.wav" );

	PRECACHE_SOUND( "weapons/gauss2.wav" );
	PRECACHE_SOUND( "weapons/electro4.wav" );
	PRECACHE_SOUND( "weapons/electro5.wav" );
	PRECACHE_SOUND( "weapons/electro6.wav" );
	PRECACHE_SOUND( "ambience/pulsemachine.wav" );

	m_iGlow = PRECACHE_MODEL( "sprites/hotglow.spr" );
	m_iBalls = PRECACHE_MODEL( "sprites/hotglow.spr" );
	m_iBeam = PRECACHE_MODEL( "sprites/smoke.spr" );

	m_usGaussFire = PRECACHE_EVENT( 1, "events/gauss.sc" );
	m_usGaussSpin = PRECACHE_EVENT( 1, "events/gaussspin.sc" );
}

int CGauss::AddToPlayer( CBasePlayer *pPlayer )
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

int CGauss::GetItemInfo( ItemInfo *p )
{
	p->pszName = STRING( pev->classname );
	p->pszAmmo1 = "uranium";
	p->iMaxAmmo1 = URANIUM_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 3;
	p->iPosition = 1;
	p->iId = m_iId = WEAPON_GAUSS;
	p->iFlags = 0;
	p->iWeight = GAUSS_WEIGHT;

	return 1;
}

BOOL CGauss::Deploy()
{
	m_pPlayer->m_flPlayAftershock = 0.0;
	return DefaultDeploy( "models/v_gauss.mdl", "models/p_gauss.mdl", GAUSS_DRAW, "gauss" );
}

void CGauss::Holster( int skiplocal /* = 0 */ )
{
	PLAYBACK_EVENT_FULL( FEV_RELIABLE | FEV_GLOBAL, m_pPlayer->edict(), m_usGaussFire, 0.01f, m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0.0, 0.0, 0, 0, 0, 1 );

	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5f;

	SendWeaponAnim( GAUSS_HOLSTER );
	m_fInAttack = 0;
}

void CGauss::PrimaryAttack()
{
	// don't fire underwater
	if( m_pPlayer->pev->waterlevel == 3 )
	{
		PlayEmptySound();
		m_flNextSecondaryAttack = m_flNextPrimaryAttack = GetNextAttackDelay( 0.15f );
		return;
	}

	if( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] < 2 )
	{
		PlayEmptySound();
		m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5f;
		return;
	}

	m_pPlayer->m_iWeaponVolume = GAUSS_PRIMARY_FIRE_VOLUME;
	m_fPrimaryFire = TRUE;

	m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] -= 2;

	StartFire();
	m_fInAttack = 0;
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.0f;
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.2f;
}

void CGauss::SecondaryAttack()
{
	// don't fire underwater
	if( m_pPlayer->pev->waterlevel == 3 )
	{
		if( m_fInAttack != 0 )
		{
			EMIT_SOUND_DYN( ENT( m_pPlayer->pev ), CHAN_WEAPON, "weapons/electro4.wav", 1.0, ATTN_NORM, 0, 80 + RANDOM_LONG( 0, 0x3f ) );
			SendWeaponAnim( GAUSS_IDLE );
			m_fInAttack = 0;
		}
		else
		{
			PlayEmptySound();
		}

		m_flNextSecondaryAttack = m_flNextPrimaryAttack = GetNextAttackDelay( 0.5f );
		return;
	}

	if( m_fInAttack == 0 )
	{
		if( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 )
		{
			EMIT_SOUND( ENT( m_pPlayer->pev ), CHAN_WEAPON, "weapons/357_cock1.wav", 0.8, ATTN_NORM );
			m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5f;
			return;
		}

		m_fPrimaryFire = FALSE;

		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;// take one ammo just to start the spin
		m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase();

		// spin up
		m_pPlayer->m_iWeaponVolume = GAUSS_PRIMARY_CHARGE_VOLUME;

		SendWeaponAnim( GAUSS_SPINUP );
		m_fInAttack = 1;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.5f;
		m_pPlayer->m_flStartCharge = gpGlobals->time;
		m_pPlayer->m_flAmmoStartCharge = UTIL_WeaponTimeBase() + GetFullChargeTime();

		PLAYBACK_EVENT_FULL( FEV_NOTHOST, m_pPlayer->edict(), m_usGaussSpin, 0.0f, g_vecZero, g_vecZero, 0.0f, 0.0f, 110, 0, 0, 0 );

		m_iSoundState = SND_CHANGE_PITCH;
	}
	else if( m_fInAttack == 1 )
	{
		if( m_flTimeWeaponIdle < UTIL_WeaponTimeBase() )
		{
			SendWeaponAnim( GAUSS_SPIN );
			m_fInAttack = 2;
		}
	}
	else
	{
		// during the charging process, eat one bit of ammo every once in a while
		if( UTIL_WeaponTimeBase() >= m_pPlayer->m_flNextAmmoBurn && m_pPlayer->m_flNextAmmoBurn != 1000 )
		{
			if( bIsMultiplayer() )
			{
				m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;
				m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase() + 0.1f;
			}
			else
			{
				m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;
				m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase() + 0.3f;
			}
		}

		if( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 )
		{
			// out of ammo! force the gun to fire
			StartFire();
			m_fInAttack = 0;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.0f;
			m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 1;
			return;
		}

		if( UTIL_WeaponTimeBase() >= m_pPlayer->m_flAmmoStartCharge )
		{
			// don't eat any more ammo after gun is fully charged.
			m_pPlayer->m_flNextAmmoBurn = 1000;
		}

		int pitch = (int)( ( gpGlobals->time - m_pPlayer->m_flStartCharge ) * ( 150 / GetFullChargeTime() ) + 100 );
		if( pitch > 250 ) 
			 pitch = 250;
		
		// ALERT( at_console, "%d %d %d\n", m_fInAttack, m_iSoundState, pitch );

		const bool overcharge = m_pPlayer->m_flStartCharge < gpGlobals->time - 10.0f;

		if( m_iSoundState == 0 )
			ALERT( at_console, "sound state %d\n", m_iSoundState );

		PLAYBACK_EVENT_FULL( FEV_NOTHOST, m_pPlayer->edict(), m_usGaussSpin, 0.0f, g_vecZero, g_vecZero, 0.0f, 0.0f, pitch, 0, ( m_iSoundState == SND_CHANGE_PITCH ) ? 1 : 0, 0 );

		m_iSoundState = SND_CHANGE_PITCH; // hack for going through level transitions

		m_pPlayer->m_iWeaponVolume = GAUSS_PRIMARY_CHARGE_VOLUME;

		// m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.1f;
		if( overcharge )
		{
			// Player charged up too long. Zap him.
			EMIT_SOUND_DYN( ENT( m_pPlayer->pev ), CHAN_WEAPON, "weapons/electro4.wav", 1.0f, ATTN_NORM, 0, 80 + RANDOM_LONG( 0, 0x3f ) );
			EMIT_SOUND_DYN( ENT( m_pPlayer->pev ), CHAN_ITEM, "weapons/electro6.wav", 1.0f, ATTN_NORM, 0, 75 + RANDOM_LONG( 0, 0x3f ) );

			m_fInAttack = 0;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.0f;
			m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 1.0f;

			SendWeaponAnim( GAUSS_IDLE );

			// Player may have been killed and this weapon dropped, don't execute any more code after this!
			return;
		}
	}
}

//=========================================================
// StartFire- since all of this code has to run and then 
// call Fire(), it was easier at this point to rip it out 
// of weaponidle() and make its own function then to try to
// merge this into Fire(), which has some identical variable names 
//=========================================================
void CGauss::StartFire( void )
{
	float flDamage;

	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );
	Vector vecAiming = gpGlobals->v_forward;
	Vector vecSrc = m_pPlayer->GetGunPosition(); // + gpGlobals->v_up * -8 + gpGlobals->v_right * 8;

	if( gpGlobals->time - m_pPlayer->m_flStartCharge > GetFullChargeTime() )
	{
		flDamage = 200.0f;
	}
	else
	{
		flDamage = 200.0f * ( ( gpGlobals->time - m_pPlayer->m_flStartCharge ) / GetFullChargeTime() );
	}

	if( m_fPrimaryFire )
	{
		// fixed damage on primary attack
		flDamage = 20.0f;
	}

	if( m_fInAttack != 3 )
	{
		//ALERT( at_console, "Time:%f Damage:%f\n", gpGlobals->time - m_pPlayer->m_flStartCharge, flDamage );

		// player "shoot" animation
		m_pPlayer->SetAnimation( PLAYER_ATTACK1 );
	}

	// time until aftershock 'static discharge' sound
	m_pPlayer->m_flPlayAftershock = gpGlobals->time + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 0.3f, 0.8f );

	Fire( vecSrc, vecAiming, flDamage );
}

void CGauss::Fire( Vector vecOrigSrc, Vector vecDir, float flDamage )
{
	m_pPlayer->m_iWeaponVolume = GAUSS_PRIMARY_FIRE_VOLUME;
	TraceResult tr, beam_tr;

	if( m_fPrimaryFire == false )
		 g_irunninggausspred = true;

	// The main firing event is sent unreliably so it won't be delayed.
	PLAYBACK_EVENT_FULL( FEV_NOTHOST, m_pPlayer->edict(), m_usGaussFire, 0.0f, m_pPlayer->pev->origin, m_pPlayer->pev->angles, flDamage, 0.0, 0, 0, m_fPrimaryFire ? 1 : 0, 0 );

	// This reliable event is used to stop the spinning sound
	// It's delayed by a fraction of second to make sure it is delayed by 1 frame on the client
	// It's sent reliably anyway, which could lead to other delays

	PLAYBACK_EVENT_FULL( FEV_NOTHOST | FEV_RELIABLE | FEV_GLOBAL, m_pPlayer->edict(), m_usGaussFire, 0.01f, m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0.0, 0.0, 0, 0, 0, 1 );

	/*ALERT( at_console, "%f %f %f\n%f %f %f\n", 
		vecSrc.x, vecSrc.y, vecSrc.z, 
		vecDest.x, vecDest.y, vecDest.z );*/

	//ALERT( at_console, "%f %f\n", tr.flFraction, flMaxFrac );

	// ALERT( at_console, "%d bytes\n", nTotal );
}

void CGauss::WeaponIdle( void )
{
	ResetEmptySound();

	// play aftershock static discharge
	if( m_pPlayer->m_flPlayAftershock && m_pPlayer->m_flPlayAftershock < gpGlobals->time )
	{
		switch( RANDOM_LONG( 0, 3 ) )
		{
		case 0:
			EMIT_SOUND( ENT( m_pPlayer->pev ), CHAN_WEAPON, "weapons/electro4.wav", RANDOM_FLOAT( 0.7f, 0.8f ), ATTN_NORM );
			break;
		case 1:
			EMIT_SOUND( ENT( m_pPlayer->pev ), CHAN_WEAPON, "weapons/electro5.wav", RANDOM_FLOAT( 0.7f, 0.8f ), ATTN_NORM );
			break;
		case 2:
			EMIT_SOUND( ENT( m_pPlayer->pev ), CHAN_WEAPON, "weapons/electro6.wav", RANDOM_FLOAT( 0.7f, 0.8f ), ATTN_NORM );
			break;
		case 3:
			break; // no sound
		}
		m_pPlayer->m_flPlayAftershock = 0.0f;
	}

	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_fInAttack != 0 )
	{
		StartFire();
		m_fInAttack = 0;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 2.0f;
	}
	else
	{
		int iAnim;
		float flRand = RANDOM_FLOAT( 0.0f, 1.0f );
		if( flRand <= 0.5f )
		{
			iAnim = GAUSS_IDLE;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10.0f, 15.0f );
		}
		else if( flRand <= 0.75f )
		{
			iAnim = GAUSS_IDLE2;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10.0f, 15.0f );
		}
		else
		{
			iAnim = GAUSS_FIDGET;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 3.0f;
		}
	}
}

class CGaussAmmo : public CBasePlayerAmmo
{
	void Spawn( void )
	{ 
		Precache();
		SET_MODEL( ENT( pev ), "models/w_gaussammo.mdl" );
		CBasePlayerAmmo::Spawn();
	}
	void Precache( void )
	{
		PRECACHE_MODEL( "models/w_gaussammo.mdl" );
		PRECACHE_SOUND( "items/9mmclip1.wav" );
	}
	BOOL AddAmmo( CBaseEntity *pOther ) 
	{ 
		if( pOther->GiveAmmo( AMMO_URANIUMBOX_GIVE, "uranium", URANIUM_MAX_CARRY ) != -1 )
		{
			EMIT_SOUND( ENT( pev ), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM );
			return TRUE;
		}
		return FALSE;
	}
};

LINK_ENTITY_TO_CLASS( ammo_gaussclip, CGaussAmmo )
