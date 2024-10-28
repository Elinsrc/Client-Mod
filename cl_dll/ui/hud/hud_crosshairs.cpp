#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "hud_crosshairs.h"

int CHudCrosshairs::Init()
{
	m_iFlags = HUD_ACTIVE;

	cl_cross = CVAR_CREATE("cl_cross", "0", FCVAR_ARCHIVE);
	cl_cross_color = CVAR_CREATE("cl_cross_color", "255 255 255", FCVAR_ARCHIVE);
	cl_cross_alpha = CVAR_CREATE("cl_cross_alpha", "255", FCVAR_ARCHIVE);
	cl_cross_gap = CVAR_CREATE("cl_cross_gap", "1", FCVAR_ARCHIVE);
	cl_cross_size = CVAR_CREATE("cl_cross_size", "2", FCVAR_ARCHIVE);
	cl_cross_thickness = CVAR_CREATE("cl_cross_thickness", "5", FCVAR_ARCHIVE);
	cl_cross_outline = CVAR_CREATE( "cl_cross_outline", "1", FCVAR_ARCHIVE );
	cl_cross_top_line = CVAR_CREATE("cl_cross_top_line", "1", FCVAR_ARCHIVE);
	cl_cross_bottom_line = CVAR_CREATE("cl_cross_bottom_line", "1", FCVAR_ARCHIVE);
	cl_cross_left_line = CVAR_CREATE("cl_cross_left_line", "1", FCVAR_ARCHIVE);
	cl_cross_right_line = CVAR_CREATE("cl_cross_right_line", "1", FCVAR_ARCHIVE);
	cl_cross_dot_size  = CVAR_CREATE("cl_cross_dot_size", "1", FCVAR_ARCHIVE);
	cl_cross_dot_color = CVAR_CREATE("cl_cross_dot_color", "255 255 255", FCVAR_ARCHIVE);

	gHUD.AddHudElem( this );
	return 0;
}

int CHudCrosshairs::VidInit()
{
	return 1;
}

int CHudCrosshairs::Draw(float time)
{
	if (cl_cross->value == 0.0f)
	{
		gEngfuncs.pfnClientCmd( "crosshair 1" );
		return 0;
	}
	else
		gEngfuncs.pfnClientCmd( "crosshair 0" );

	// Original code
	// from https://www.hlfx.ru/forum/showthread.php?s=df9c094f8744c0c1dbff989dc2c2b90f&threadid=3010&postid=83364#post83364
	// Edited Elinsrc

	int isize = XRES(cl_cross_size->value);
	int iLength = XRES(cl_cross_thickness->value);
	int iDepth = XRES(cl_cross_gap->value);
	int dotsize = XRES(cl_cross_dot_size->value);
	int xPos = ScreenWidth/2;
	int yPos = ScreenHeight/2;
	int outline = cl_cross_outline->value;;
	int r, g, b;
	int alpha = cl_cross_alpha->value;

	const char *cross_color = cl_cross_color->string;
	if (sscanf( cross_color, "%d %d %d", &r, &g, &b) == 3)
	{
		r = Q_max(r, 0);
		g = Q_max(g, 0);
		b = Q_max(b, 0);

		r = Q_min(r, 255);
		g = Q_min(g, 255);
		b = Q_min(b, 255);
	}
	else
	{
		r = 255;
		g = 255;
		b = 255;
	}

	if ((cl_cross_outline->value > 0.0f) && (cl_cross_thickness->value > 0.0f))
	{
		// Top outline
		if (cl_cross_top_line->value)
			FillRGBABlend(xPos - iDepth/2 - outline, yPos - isize - iLength - outline, iDepth + outline * 2, iLength + outline * 2, 0, 0, 0, alpha);

		// Bottom outline
		if (cl_cross_bottom_line->value)
			FillRGBABlend(xPos - iDepth/2 -outline , yPos + isize - outline , iDepth + outline * 2, iLength + outline * 2, 0, 0, 0, alpha);

		// Left outline
		if (cl_cross_left_line->value)
			FillRGBABlend(xPos - isize - iLength - outline, yPos - iDepth/2 - outline, iLength + outline * 2, iDepth + outline * 2, 0, 0, 0, alpha);

		// Right outline
		if (cl_cross_right_line->value)
			FillRGBABlend(xPos + isize - outline, yPos - iDepth/2 - outline, iLength + outline * 2, iDepth + outline * 2, 0, 0, 0, alpha);

		// Dot outline
		if (cl_cross_dot_size->value > 0.0f)
			FillRGBABlend(xPos - dotsize/2 - outline, yPos - dotsize/2 - outline, dotsize + outline * 2, dotsize + outline * 2, 0, 0, 0, alpha);
	}

	if (cl_cross_thickness->value > 0.0f)
	{
		// Top line
		if (cl_cross_top_line->value)
			FillRGBABlend(xPos - iDepth/2, yPos - isize - iLength, iDepth, iLength, r, g, b, alpha);

		// Bottom line
		if (cl_cross_bottom_line->value)
			FillRGBABlend(xPos - iDepth/2, yPos + isize, iDepth, iLength, r, g, b, alpha);

		// Left line
		if (cl_cross_left_line->value)
			FillRGBABlend(xPos - isize - iLength, yPos - iDepth/2, iLength, iDepth, r, g, b, alpha);

		// Right line
		if (cl_cross_right_line->value)
			FillRGBABlend(xPos + isize, yPos - iDepth/2, iLength, iDepth, r, g, b, alpha);
	}

	const char *dot_color = cl_cross_dot_color->string;
	if (sscanf( dot_color, "%d %d %d", &r, &g, &b) == 3)
	{
		r = Q_max(r, 0);
		g = Q_max(g, 0);
		b = Q_max(b, 0);

		r = Q_min(r, 255);
		g = Q_min(g, 255);
		b = Q_min(b, 255);
	}
	else
	{
		r = 255;
		g = 255;
		b = 255;
	}

	// Dot
	if ((cl_cross_thickness->value > 0.0f) && (cl_cross_dot_size->value > 0.0f))
		FillRGBABlend(xPos - dotsize/2, yPos - dotsize/2, dotsize, dotsize, r, g, b, alpha);

	return 0;
}
