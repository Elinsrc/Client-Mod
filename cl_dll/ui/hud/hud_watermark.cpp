// OpenAG
#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "build_info.h"

int CHudWatermark::Init()
{
	m_iFlags = 0;

	hud_watermark = CVAR_CREATE("hud_watermark", "1", FCVAR_ARCHIVE);

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
	if (hud_watermark->value == 0.0f)
		return 0;

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

	char build[256];
	sprintf(build, "Client-Mod: built %s, commit %s, architecture %s, platform %s",
		BuildInfo::GetDate(),
		BuildInfo::GetCommitHash(),
		BuildInfo::GetArchitecture(),
		BuildInfo::GetPlatform()
	);

	DrawString(ScreenWidth / 20, gHUD.m_scrinfo.iCharHeight, build, r, g, b);
	DrawString(ScreenWidth / 20, gHUD.m_scrinfo.iCharHeight * 2, "t.me/Elinsrc", r, g, b);
	DrawString(ScreenWidth / 20, gHUD.m_scrinfo.iCharHeight * 3, "t.me/HalfLifeCollectiveChat", r, g, b);
	DrawString(ScreenWidth / 20, gHUD.m_scrinfo.iCharHeight * 4, BuildInfo::GetGitHubLink(), r, g, b);
	DrawString(ScreenWidth / 20, gHUD.m_scrinfo.iCharHeight * 6, "To disable this message, type in the console 'hud_watermark 0'", r, g, b);

	return 0;
}
