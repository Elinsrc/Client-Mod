// OpenAG
#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"

int CHudWatermark::Init()
{
	m_iFlags = 0;

	gHUD.AddHudElem(this);
	return 0;
}

int CHudWatermark::VidInit()
{
	m_iFlags |= HUD_ACTIVE;
	refresh_draw_until = true;

	return 1;
}

int CHudWatermark::Draw(float time)
{
	if (refresh_draw_until || (draw_until > gHUD.m_flTime + 15.0f)) {
		refresh_draw_until = false;
		draw_until = gHUD.m_flTime + 15.0f;
	}

	if (gHUD.m_flTime >= draw_until) {
		m_iFlags &= ~HUD_ACTIVE;
		return 0;
	}

	int r, g, b;
	UnpackRGB(r, g, b, gHUD.m_iDefaultHUDColor);

	DrawString(ScreenWidth / 20, gHUD.m_scrinfo.iCharHeight, "Client-Mod build " __DATE__, r, g, b);
	DrawString(ScreenWidth / 20, gHUD.m_scrinfo.iCharHeight * 2, "t.me/Elinsrc", r, g, b);
    DrawString(ScreenWidth / 20, gHUD.m_scrinfo.iCharHeight * 3, "t.me/HalfLifeCollectiveChat", r, g, b);

	return 0;
}
