#pragma once
#include <cstdint>

class CHudSpeedometer : public CHudBase
{
	cvar_t *hud_speedometer;
	cvar_t *hud_speedometer_below_cross;

public:
	virtual int Init();
	virtual int VidInit();
	virtual int Draw(float flTime);
	void UpdateSpeed(const float velocity[2]);

private:
	int m_iSpeed;
};
