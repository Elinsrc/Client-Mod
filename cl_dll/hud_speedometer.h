#pragma once
#include <cstdint>

class CHudSpeedometer : public CHudBase
{
public:
	virtual int Init();
	virtual int VidInit();
	virtual int Draw(float time);
	void UpdateSpeed(const float velocity[2]);

private:
	int m_iSpeed;
};