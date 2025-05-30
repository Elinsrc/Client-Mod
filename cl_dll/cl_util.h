/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
// cl_util.h
//
#if !defined(CL_UTIL_H)
#define CL_UTIL_H
#include "exportdef.h"
#include "cvardef.h"
#include "net_api.h"
#include "color_tags.h"

#include <stdio.h> // for safe_sprintf()
#include <stdarg.h>  // "
#include <string> // for strncpy()

#if !defined(TRUE)
#define TRUE 1
#define FALSE 0
#endif

// Macros to hook function calls into the HUD object

#define HOOK_MESSAGE(x) gEngfuncs.pfnHookUserMsg( #x, __MsgFunc_##x );

#define DECLARE_MESSAGE(y, x) int __MsgFunc_##x( const char *pszName, int iSize, void *pbuf ) \
						{ \
							return gHUD.y.MsgFunc_##x(pszName, iSize, pbuf ); \
						}

#define HOOK_COMMAND(x, y) gEngfuncs.pfnAddCommand( x, __CmdFunc_##y );
#define DECLARE_COMMAND(y, x) void __CmdFunc_##x( void ) \
							{ \
								gHUD.y.UserCmd_##x( ); \
							}

inline float CVAR_GET_FLOAT( const char *x ) {	return gEngfuncs.pfnGetCvarFloat( (char*)x ); }
inline char* CVAR_GET_STRING( const char *x ) {	return gEngfuncs.pfnGetCvarString( (char*)x ); }
inline struct cvar_s *CVAR_CREATE( const char *cv, const char *val, const int flags ) {	return gEngfuncs.pfnRegisterVariable( (char*)cv, (char*)val, flags ); }

#define SPR_Load ( *gEngfuncs.pfnSPR_Load )
#define SPR_Set ( *gEngfuncs.pfnSPR_Set )
#define SPR_Frames ( *gEngfuncs.pfnSPR_Frames )
#define SPR_GetList ( *gEngfuncs.pfnSPR_GetList )

// SPR_Draw  draws a the current sprite as solid
#define SPR_Draw ( *gEngfuncs.pfnSPR_Draw )
// SPR_DrawHoles  draws the current sprites, with color index255 not drawn (transparent)
#define SPR_DrawHoles ( *gEngfuncs.pfnSPR_DrawHoles )
// SPR_DrawAdditive  adds the sprites RGB values to the background  (additive transulency)
#define SPR_DrawAdditive ( *gEngfuncs.pfnSPR_DrawAdditive )

// SPR_EnableScissor  sets a clipping rect for HUD sprites. (0,0) is the top-left hand corner of the screen.
#define SPR_EnableScissor ( *gEngfuncs.pfnSPR_EnableScissor )
// SPR_DisableScissor  disables the clipping rect
#define SPR_DisableScissor ( *gEngfuncs.pfnSPR_DisableScissor )
//
#define FillRGBA ( *gEngfuncs.pfnFillRGBA )
#define FillRGBABlend ( *gEngfuncs.pfnFillRGBABlend )

// ScreenHeight returns the height of the screen, in pixels
#define ScreenHeight ( gHUD.m_scrinfo.iHeight )
// ScreenWidth returns the width of the screen, in pixels
#define ScreenWidth ( gHUD.m_scrinfo.iWidth )

// Use this to set any co-ords in 640x480 space
#define XRES(x)		( (int)( float(x) * ( (float)ScreenWidth / 640.0f ) + 0.5f ) )
#define YRES(y)		( (int)( float(y) * ( (float)ScreenHeight / 480.0f ) + 0.5f ) )
#define XRES_HD(x)	( (int)( float(x) * Q_max(1.f, (float)ScreenWidth / 1280.f )))
#define YRES_HD(y)	( (int)( float(y) * Q_max(1.f, (float)ScreenHeight / 720.f )))

// use this to project world coordinates to screen coordinates
#define XPROJECT(x)	( ( 1.0f + (x) ) * ScreenWidth * 0.5f )
#define YPROJECT(y)	( ( 1.0f - (y) ) * ScreenHeight * 0.5f )

#define GetScreenInfo ( *gEngfuncs.pfnGetScreenInfo )
#define ServerCmd ( *gEngfuncs.pfnServerCmd )
#define ClientCmd ( *gEngfuncs.pfnClientCmd )
#define SetCrosshair ( *gEngfuncs.pfnSetCrosshair )
#define AngleVectors ( *gEngfuncs.pfnAngleVectors )

// Gets the height & width of a sprite,  at the specified frame
inline int SPR_Height( HSPRITE x, int f )	{ return gEngfuncs.pfnSPR_Height(x, f); }
inline int SPR_Width( HSPRITE x, int f )	{ return gEngfuncs.pfnSPR_Width(x, f); }

inline client_textmessage_t *TextMessageGet( const char *pName )
{
	return gEngfuncs.pfnTextMessageGet( pName );
}

inline int TextMessageDrawChar( int x, int y, int number, int r, int g, int b ) 
{
	return gEngfuncs.pfnDrawCharacter( x, y, number, r, g, b ); 
}

int DrawUtfString( int xpos, int ypos, int iMaxX, const char *szIt, int r, int g, int b );

inline int DrawConsoleString( int x, int y, char *string )
{
	return gHUD.DrawConsoleStringWithColorTags(x, y, string);
}

inline void GetConsoleStringSize( char *string, int *width, int *height )
{
	gHUD.GetConsoleStringSizeWithColorTags(string, *width, *height);
}

inline int ConsoleStringLen( char *string )
{
	int _width, _height;
	GetConsoleStringSize( string, &_width, &_height );
	return _width;
}

inline void ConsolePrint( const char *string )
{
	gEngfuncs.pfnConsolePrint( color_tags::strip_color_tags_thread_unsafe(string) );
}

inline void CenterPrint( const char *string )
{
	gEngfuncs.pfnCenterPrint( color_tags::strip_color_tags_thread_unsafe(string) );
}

inline int DrawString( int x, int y, const char *string, int r, int g, int b )
{
	return gEngfuncs.pfnDrawString( x, y, (char*) string, r, g, b );
}

// returns the players name of entity no.
#define GetPlayerInfo ( *gEngfuncs.pfnGetPlayerInfo )

// sound functions
inline void PlaySound( const char *szSound, float vol ) { gEngfuncs.pfnPlaySoundByName( szSound, vol ); }
inline void PlaySound( int iSound, float vol ) { gEngfuncs.pfnPlaySoundByIndex( iSound, vol ); }

#define Q_max(a, b)  (((a) > (b)) ? (a) : (b))
#define Q_min(a, b)  (((a) < (b)) ? (a) : (b))
#define fabs(x)	   ((x) > 0 ? (x) : 0 - (x))

inline int GetSpriteRes( int width, int height )
{
	int i;

	if( width < 640 )
		i = 320;
	else if( width < 1280 || !gHUD.m_pAllowHD->value )
		i = 640;
	else
	{
		if( height <= 720 )
			i = 640;
		else if( width <= 2560 || height <= 1600 )
			i = 1280;
		else
			i = 2560;
	}

	return Q_min( i, gHUD.m_iMaxRes );
}

// OpenAG
template<typename T, size_t N>
char (&ArraySizeHelper(T (&)[N]))[N];
#define ARRAYSIZE(x) sizeof(ArraySizeHelper(x))

static size_t count_digits(int n)
{
	size_t result = 0;

	do {
		++result;
	} while ((n /= 10) != 0);

	return result;
}

static char* get_server_address()
{
	net_status_t netstatus{};
	gEngfuncs.pNetAPI->Status(&netstatus);
	return gEngfuncs.pNetAPI->AdrToString(&netstatus.remote_address);
}

static void sanitize_address(std::string& address)
{
	for (size_t i = 0; i < address.size(); ++i) {
		char c = address[i];
		if ((c >= '0' && c <= '9') || c == '.' || c == ':')
			continue;

		// Invalid character.
		address = address.substr(0, i);
		break;
	}
}

static size_t get_map_name(char* dest, size_t count)
{
	auto map_path = gEngfuncs.pfnGetLevelName();

	const char* slash = strrchr(map_path, '/');
	if (!slash)
		slash = map_path - 1;

	const char* dot = strrchr(map_path, '.');
	if (!dot)
		dot = map_path + strlen(map_path);

	size_t bytes_to_copy = Q_min(count - 1, static_cast<size_t>(dot - slash - 1));

	strncpy(dest, slash + 1, bytes_to_copy);
	dest[bytes_to_copy] = '\0';

	return bytes_to_copy;
}

static size_t get_player_count()
{
	size_t player_count = 0;

	for (int i = 0; i < MAX_PLAYERS; ++i) {
		// Make sure the information is up to date.
		gEngfuncs.pfnGetPlayerInfo(i + 1, &g_PlayerInfoList[i + 1]);

		// This player slot is empty.
		if (g_PlayerInfoList[i + 1].name == nullptr)
			continue;

		++player_count;
	}

	return player_count;
}

inline void convert_to_lower_case(const char *str)
{
	unsigned char *str_lw = (unsigned char *)str;
	while (*str_lw) {
		*str_lw = tolower(*str_lw);
		str_lw++;
	}
}

inline char *safe_strcpy( char *dst, const char *src, int len_dst)
{
	if( len_dst <= 0 )
	{
		return NULL; // this is bad
	}

	strncpy(dst,src,len_dst);
	dst[ len_dst - 1 ] = '\0';

	return dst;
}

inline int safe_sprintf( char *dst, int len_dst, const char *format, ...)
{
	if( len_dst <= 0 )
	{
		return -1; // this is bad
	}

	va_list v;

    va_start(v, format);

	_vsnprintf(dst,len_dst,format,v);

	va_end(v);

	dst[ len_dst - 1 ] = '\0';

	return 0;
}
//

inline void remove_color_characters(char *input_string) {
	char *read = input_string;
	char *write = input_string;

	while (*read) {
		if (*read == '^' && isdigit(*(read + 1))) {
			read += 2;
		} else {
			*write++ = *read++;
		}
	}
	*write = '\0';
}

void ScaleColors( int &r, int &g, int &b, int a );

#define DotProduct(x, y) ((x)[0] * (y)[0] + (x)[1] * (y)[1] + (x)[2] * (y)[2])
#define VectorSubtract(a, b, c) { (c)[0] = (a)[0] - (b)[0]; (c)[1] = (a)[1] - (b)[1]; (c)[2] = (a)[2] - (b)[2]; }
#define VectorAdd(a, b, c) { (c)[0] = (a)[0] + (b)[0]; (c)[1] = (a)[1] + (b)[1]; (c)[2] = (a)[2] + (b)[2]; }
#define VectorCopy(a, b) { (b)[0] = (a)[0]; (b)[1] = (a)[1]; (b)[2] = (a)[2]; }
inline void VectorClear( float *a ) { a[0] = 0.0; a[1] = 0.0; a[2] = 0.0; }
float Length( const float *v );
void VectorMA( const float *veca, float scale, const float *vecb, float *vecc );
void VectorScale( const float *in, float scale, float *out );
float VectorNormalize( float *v );
void VectorInverse( float *v );

extern vec3_t vec3_origin;

// disable 'possible loss of data converting float to int' warning message
#pragma warning( disable: 4244 )
// disable 'truncation from 'const double' to 'float' warning message
#pragma warning( disable: 4305 )

inline void UnpackRGB( int &r, int &g, int &b, unsigned long ulRGB )\
{\
	r = ( ulRGB & 0xFF0000 ) >> 16;\
	g = ( ulRGB & 0xFF00 ) >> 8;\
	b = ulRGB & 0xFF;\
}

HSPRITE LoadSprite( const char *pszName );

bool HUD_MessageBox( const char *msg );
bool IsXashFWGS();
#endif
