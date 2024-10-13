#pragma once

class CHudWatermark : public CHudBase
{
	cvar_t *hud_watermark;

	float draw_until;
	bool refresh_draw_until;

public:
	virtual int Init();
	virtual int VidInit();
	virtual int Draw(float flTime);
};
