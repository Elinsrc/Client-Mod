#include <cmath>

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "hud_crosshair.h"

cvar_t *cl_cross;
cvar_t *cl_cross_color;
cvar_t *cl_cross_trick;
cvar_t *cl_cross_dist;
cvar_t *cl_cross_lenght;
cvar_t *cl_cross_alpha;

int CHudCrosshair::Init()
{
	m_iFlags = HUD_ACTIVE;

	cl_cross = CVAR_CREATE("cl_cross", "0", FCVAR_ARCHIVE);
	cl_cross_color = CVAR_CREATE("cl_cross_color", "0 255 0", FCVAR_ARCHIVE);
	cl_cross_alpha = CVAR_CREATE("cl_cross_alpha", "200", FCVAR_ARCHIVE);
	cl_cross_trick = CVAR_CREATE("cl_cross_trick", "1", FCVAR_ARCHIVE);
	cl_cross_dist = CVAR_CREATE("cl_cross_dist", "2", FCVAR_ARCHIVE);
	cl_cross_lenght = CVAR_CREATE("cl_cross_lenght", "5", FCVAR_ARCHIVE);

	gHUD.AddHudElem( this );
	return 0;
}

int CHudCrosshair::VidInit()
{
	return 1;
}

int CHudCrosshair::Draw(float time)
{
	if( !( gHUD.m_iWeaponBits & ( 1 << ( WEAPON_SUIT ) ) ) )
		return 1;
	
	if (cl_cross->value == 0.0f)
	{
		gEngfuncs.pfnClientCmd( "crosshair 1" );
		return 0;
	}
	else
		gEngfuncs.pfnClientCmd( "crosshair 0" );

	int iDist = XRES(cl_cross_dist->value);
	int iLength = XRES(cl_cross_lenght->value);
	int iDepth = XRES(cl_cross_trick->value);
	int xPos = ScreenWidth/2;
	int yPos = ScreenHeight/2;

	int r, g, b;
	int a = cl_cross_alpha->value;
	const char *colors = cl_cross_color->string;
	sscanf( colors, "%d %d %d", &r, &g, &b);

	if (cl_cross->value == 1)
	{
		FillRGBA(xPos - iDepth/2, yPos - iDepth/2, iDepth, iDepth, r, g, b, a); // center
		FillRGBA(xPos - iDepth/2, yPos - iDist - iLength, iDepth, iLength, r, g, b, a); // top
		FillRGBA(xPos - iDepth/2, yPos + iDist, iDepth, iLength, r, g, b, a); // bottom
		FillRGBA(xPos - iDist - iLength, yPos - iDepth/2, iLength, iDepth, r, g, b, a); // left
		FillRGBA(xPos + iDist, yPos - iDepth/2, iLength, iDepth, r, g, b, a); // right
	}
	else if (cl_cross->value == 2)
	{
		FillRGBA(xPos - iDepth/2, yPos - iDist - iLength, iDepth, iLength, r, g, b, a); // top
		FillRGBA(xPos - iDepth/2, yPos + iDist, iDepth, iLength, r, g, b, a); // bottom
		FillRGBA(xPos - iDist - iLength, yPos - iDepth/2, iLength, iDepth, r, g, b, a); // left
		FillRGBA(xPos + iDist, yPos - iDepth/2, iLength, iDepth, r, g, b, a); // right
	}
	else if (cl_cross->value == 3)
	{
		FillRGBA(xPos - iDist - iLength, yPos - iDepth/2, iLength, iDepth, r, g, b, a); // left
		FillRGBA(xPos + iDist, yPos - iDepth/2, iLength, iDepth, r, g, b, a); // right
	}
	else if (cl_cross->value == 4)
	{
		FillRGBA(xPos - iDepth/2, yPos - iDepth/2, iDepth, iDepth, r, g, b, a); // center
	}	
	
	return 0;
}