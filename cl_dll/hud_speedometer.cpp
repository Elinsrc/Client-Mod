#include <cmath>

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "hud_speedometer.h"

cvar_t *hud_speedometer;
cvar_t *hud_speedometer_below_cross;
cvar_t *cl_colored_speed;
cvar_t *hud_speedcolor;

int CHudSpeedometer::Init()
{
	m_iFlags = HUD_ACTIVE;

	hud_speedometer = CVAR_CREATE("hud_speedometer", "1", FCVAR_ARCHIVE);
	hud_speedometer_below_cross = CVAR_CREATE("hud_speedometer_below_cross", "0", FCVAR_ARCHIVE);
	cl_colored_speed = CVAR_CREATE("cl_colored_speed", "0", FCVAR_ARCHIVE);
	hud_speedcolor = CVAR_CREATE("hud_speedcolor", "255 255 0", FCVAR_ARCHIVE);

	gHUD.AddHudElem( this );

}

int CHudSpeedometer::VidInit()
{
}

int CHudSpeedometer::Draw(float time)
{
	if (hud_speedometer->value == 0.0f)
		return 0;

	int r, g, b;
	
	if (cl_colored_speed->value != 0.0f)
	{
	  const char *colors = hud_speedcolor->string;
	  sscanf( colors, "%d %d %d", &r, &g, &b);
	}
	else
	{
	  UnpackRGB( r, g, b, RGB_YELLOWISH );
	}

	int y;
	if (hud_speedometer_below_cross->value != 0)
		y = ScreenHeight / 2 + gHUD.m_iFontHeight / 2;
	else
		y = ScreenHeight - gHUD.m_iFontHeight - gHUD.m_iFontHeight / 2;

	gHUD.DrawHudNumberCentered(ScreenWidth / 2, y, m_iSpeed, r, g, b);
}

void CHudSpeedometer::UpdateSpeed(const float velocity[2])
{
	m_iSpeed = std::round(std::hypot(velocity[0], velocity[1]));
}