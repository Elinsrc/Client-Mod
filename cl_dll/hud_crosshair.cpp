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
cvar_t *cl_cross_stroke;
cvar_t *cl_cross_top_line;
cvar_t *cl_cross_bottom_line;
cvar_t *cl_cross_left_line;
cvar_t *cl_cross_right_line;
cvar_t *cl_cross_dot;

int CHudCrosshair::Init()
{
	m_iFlags = HUD_ACTIVE;

	cl_cross = CVAR_CREATE("cl_cross", "0", FCVAR_ARCHIVE);
	cl_cross_color = CVAR_CREATE("cl_cross_color", "255 255 255", FCVAR_ARCHIVE);
	cl_cross_alpha = CVAR_CREATE("cl_cross_alpha", "200", FCVAR_ARCHIVE);
	cl_cross_trick = CVAR_CREATE("cl_cross_trick", "1", FCVAR_ARCHIVE);
	cl_cross_dist = CVAR_CREATE("cl_cross_dist", "2", FCVAR_ARCHIVE);
	cl_cross_lenght = CVAR_CREATE("cl_cross_lenght", "5", FCVAR_ARCHIVE);
	cl_cross_stroke = CVAR_CREATE( "cl_cross_stroke", "1", FCVAR_ARCHIVE );
	cl_cross_top_line = CVAR_CREATE("cl_cross_top_line", "1", FCVAR_ARCHIVE);
	cl_cross_bottom_line = CVAR_CREATE("cl_cross_bottom_line", "1", FCVAR_ARCHIVE);
	cl_cross_left_line = CVAR_CREATE("cl_cross_left_line", "1", FCVAR_ARCHIVE);
	cl_cross_right_line = CVAR_CREATE("cl_cross_right_line", "1", FCVAR_ARCHIVE);
	cl_cross_dot = CVAR_CREATE("cl_cross_dot", "1", FCVAR_ARCHIVE);


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

	//code written by Ku2zoff
	int iDist = XRES(cl_cross_dist->value);
	int iLength = XRES(cl_cross_lenght->value);
	int iDepth = XRES(cl_cross_trick->value);
	int xPos = ScreenWidth/2;
	int yPos = ScreenHeight/2;
	int stroke = cl_cross_stroke->value;

	int r, g, b;
	int a = cl_cross_alpha->value;
	const char *colors = cl_cross_color->string;
	sscanf( colors, "%d %d %d", &r, &g, &b);

	// Top line
	if (cl_cross_top_line->value)
	{
		if (cl_cross_stroke->value)
			FillRGBABlend(xPos - iDepth/2 - stroke, yPos - iDist - iLength - stroke, iDepth + stroke * 2, iLength + stroke * 2, 0, 0, 0, a);
		FillRGBABlend(xPos - iDepth/2, yPos - iDist - iLength, iDepth, iLength, r, g, b, a);
	}
	
	// Bottom line
	if (cl_cross_bottom_line->value)
	{
		if (cl_cross_stroke->value)
			FillRGBABlend(xPos - iDepth/2 -stroke , yPos + iDist - stroke , iDepth + stroke * 2, iLength + stroke * 2, 0, 0, 0, a);
		FillRGBABlend(xPos - iDepth/2, yPos + iDist, iDepth, iLength, r, g, b, a);
	}
	
	// Left line
	if (cl_cross_left_line->value)
	{
		if (cl_cross_stroke->value)
			FillRGBABlend(xPos - iDist - iLength - stroke, yPos - iDepth/2 - stroke, iLength + stroke * 2, iDepth + stroke * 2, 0, 0, 0, a);
		FillRGBABlend(xPos - iDist - iLength, yPos - iDepth/2, iLength, iDepth, r, g, b, a);
	}

	// Right line
	if (cl_cross_right_line->value)
	{
		if (cl_cross_stroke->value)
			FillRGBABlend(xPos + iDist - stroke, yPos - iDepth/2 - stroke, iLength + stroke * 2, iDepth + stroke * 2, 0, 0, 0, a);
		FillRGBABlend(xPos + iDist, yPos - iDepth/2, iLength, iDepth, r, g, b, a);
	}
	
	// Dot
	if (cl_cross_dot->value)
	{
		if (cl_cross_stroke->value)
			FillRGBABlend(xPos - iDepth/2 - stroke, yPos - iDepth/2 - stroke, iDepth + stroke * 2, iDepth + stroke * 2, 0, 0, 0, a);
		FillRGBABlend(xPos - iDepth/2, yPos - iDepth/2, iDepth, iDepth, r, g, b, a);
	}

	return 0;
}