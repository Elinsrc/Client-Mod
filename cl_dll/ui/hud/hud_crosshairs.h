#pragma once
#include <cstdint>

class CHudCrosshairs : public CHudBase
{
	cvar_t *cl_cross;
	cvar_t *cl_cross_color;
	cvar_t *cl_cross_gap;
	cvar_t *cl_cross_size;
	cvar_t *cl_cross_thickness;
	cvar_t *cl_cross_alpha;
	cvar_t *cl_cross_outline;
	cvar_t *cl_cross_top_line;
	cvar_t *cl_cross_bottom_line;
	cvar_t *cl_cross_left_line;
	cvar_t *cl_cross_right_line;
	cvar_t *cl_cross_dot_size;
	cvar_t *cl_cross_dot_color;

public:
	virtual int Init();
	virtual int VidInit();
	virtual int Draw(float flTime);
};
