#pragma once
#include <cstdint>

class CHudCrosshair : public CHudBase
{
public:
	virtual int Init();
	virtual int VidInit();
	virtual int Draw(float flTime);
};
