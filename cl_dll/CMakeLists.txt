#
# Copyright (c) 2016 Alibek Omarov
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

cmake_minimum_required(VERSION 2.8.12)
project (CLDLL)

set (CLDLL_LIBRARY client)

add_definitions(-DCLIENT_DLL)

if(NOT MSVC)
	add_compile_options(-fno-exceptions) # GCC/Clang flag
	add_compile_options(-Wno-write-strings) # GCC/Clang flag
	add_compile_options(-fvisibility=hidden) # GCC/Clang flag
	add_definitions(-D_LINUX -DLINUX) # It seems enough for all non-Win32 systems
	add_definitions(-Dstricmp=strcasecmp -Dstrnicmp=strncasecmp)
	if(NOT MINGW)
		add_definitions(-D_snprintf=snprintf -D_vsnprintf=vsnprintf)
	endif()
else()
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
endif()

if (GOLDSOURCE_SUPPORT)
	add_definitions(-DGOLDSOURCE_SUPPORT)
endif()

if (USE_VGUI)
	add_definitions(-DUSE_VGUI)
	if (USE_NOVGUI_MOTD)
		add_definitions(-DUSE_NOVGUI_MOTD)
	endif()
	if (USE_NOVGUI_SCOREBOARD)
		add_definitions(-DUSE_NOVGUI_SCOREBOARD)
	endif()
endif()

if(BUILD_DISCORD_RPC)
	add_definitions(-DBUILD_DISCORD_RPC)
endif()

if(BUILD_IMGUI)
	add_definitions(-DBUILD_IMGUI)
endif()

set (CLDLL_SOURCES
	${CMAKE_SOURCE_DIR}/dlls/crossbow.cpp
	${CMAKE_SOURCE_DIR}/dlls/crowbar.cpp
	${CMAKE_SOURCE_DIR}/dlls/egon.cpp
	${CMAKE_SOURCE_DIR}/dlls/gauss.cpp
	${CMAKE_SOURCE_DIR}/dlls/handgrenade.cpp
	${CMAKE_SOURCE_DIR}/dlls/hornetgun.cpp
	${CMAKE_SOURCE_DIR}/dlls/mp5.cpp
	${CMAKE_SOURCE_DIR}/dlls/python.cpp
	${CMAKE_SOURCE_DIR}/dlls/rpg.cpp
	${CMAKE_SOURCE_DIR}/dlls/satchel.cpp
	${CMAKE_SOURCE_DIR}/dlls/shotgun.cpp
	${CMAKE_SOURCE_DIR}/dlls/squeakgrenade.cpp
	${CMAKE_SOURCE_DIR}/dlls/tripmine.cpp
	${CMAKE_SOURCE_DIR}/dlls/glock.cpp
	ev_hldm.cpp
	hl/hl_baseentity.cpp
	hl/hl_events.cpp
	hl/hl_objects.cpp
	hl/hl_weapons.cpp
	ammo.cpp
	ammo_secondary.cpp
	ammohistory.cpp
	battery.cpp
	cdll_int.cpp
	com_weapons.cpp
	death.cpp
	demo.cpp
	entity.cpp
	ev_common.cpp
	events.cpp
	flashlight.cpp
	GameStudioModelRenderer.cpp
	geiger.cpp
	health.cpp
	hud.cpp
	hud_msg.cpp
	hud_redraw.cpp
	hud_spectator.cpp
	hud_speedometer.cpp
	hud_jumpspeed.cpp
	hud_update.cpp
	hud_crosshairs.cpp
	hud_watermark.cpp
	in_camera.cpp
	input.cpp
	input_goldsource.cpp
	input_mouse.cpp
	input_xash3d.cpp
	menu.cpp
	message.cpp
	parsemsg.cpp
	${CMAKE_SOURCE_DIR}/pm_shared/pm_debug.c
	${CMAKE_SOURCE_DIR}/pm_shared/pm_math.c
	${CMAKE_SOURCE_DIR}/pm_shared/pm_shared.c
	saytext.cpp
	status_icons.cpp
	statusbar.cpp
	studio_util.cpp
	StudioModelRenderer.cpp
	text_message.cpp
	train.cpp
	tri.cpp
	util.cpp
	view.cpp)

if (USE_VGUI)
	list(APPEND CLDLL_SOURCES
		vgui_int.cpp
		vgui_ClassMenu.cpp
		vgui_ConsolePanel.cpp
		vgui_ControlConfigPanel.cpp
		vgui_CustomObjects.cpp
		vgui_MOTDWindow.cpp
		vgui_SchemeManager.cpp
		vgui_ScorePanel.cpp
		vgui_TeamFortressViewport.cpp
		vgui_SpectatorPanel.cpp
		vgui_teammenu.cpp
		voice_status.cpp
		${CMAKE_SOURCE_DIR}/game_shared/vgui_checkbutton2.cpp
		${CMAKE_SOURCE_DIR}/game_shared/vgui_grid.cpp
		${CMAKE_SOURCE_DIR}/game_shared/vgui_helpers.cpp
		${CMAKE_SOURCE_DIR}/game_shared/vgui_listbox.cpp
		${CMAKE_SOURCE_DIR}/game_shared/vgui_loadtga.cpp
		${CMAKE_SOURCE_DIR}/game_shared/vgui_scrollbar2.cpp
		${CMAKE_SOURCE_DIR}/game_shared/vgui_slider2.cpp
		${CMAKE_SOURCE_DIR}/game_shared/voice_banmgr.cpp
		)
	if (USE_NOVGUI_MOTD)
		list(APPEND CLDLL_SOURCES MOTD.cpp)
	endif()
	if (USE_NOVGUI_SCOREBOARD)
		list(APPEND CLDLL_SOURCES scoreboard.cpp)
	endif()
else()
	list(APPEND CLDLL_SOURCES
		MOTD.cpp
		scoreboard.cpp)
endif()

if (BUILD_DISCORD_RPC)
	list(APPEND CLDLL_SOURCES
		discord-rpc/discord_integration.cpp)
endif()

if (BUILD_IMGUI)
	list(APPEND CLDLL_SOURCES
		"${CMAKE_SOURCE_DIR}/3rdparty/imgui/imgui.cpp"
		"${CMAKE_SOURCE_DIR}/3rdparty/imgui/imgui_demo.cpp"
		"${CMAKE_SOURCE_DIR}/3rdparty/imgui/imgui_draw.cpp"
		"${CMAKE_SOURCE_DIR}/3rdparty/imgui/imgui_tables.cpp"
		"${CMAKE_SOURCE_DIR}/3rdparty/imgui/imgui_widgets.cpp")
endif()

include_directories (. hl/ ${CMAKE_SOURCE_DIR}/dlls ${CMAKE_SOURCE_DIR}/dlls/wpn_shared ${CMAKE_SOURCE_DIR}/common ${CMAKE_SOURCE_DIR}/engine ${CMAKE_SOURCE_DIR}/pm_shared ${CMAKE_SOURCE_DIR}/game_shared ${CMAKE_SOURCE_DIR}/public)

if (BUILD_DISCORD_RPC)
	include_directories(${CMAKE_SOURCE_DIR}/3rdparty/discord-rpc/include discord-rpc)
endif()

if (BUILD_IMGUI)
	include_directories(${CMAKE_SOURCE_DIR}/3rdparty/imgui imgui)
endif()

if (USE_VGUI)
	SET(CMAKE_SKIP_RPATH TRUE)
	link_directories(${CMAKE_SOURCE_DIR}/vgui_support/vgui-dev/lib)
	include_directories(${CMAKE_SOURCE_DIR}/vgui_support/vgui-dev/include)
else()
	include_directories(${CMAKE_SOURCE_DIR}/utils/fake_vgui/include)
endif()

if(USE_VOICEMGR)
	#set(CLDLL_SOURCES
	#	${CLDLL_SOURCES}
	#	${CMAKE_SOURCE_DIR}/game_shared/voice_banmgr.cpp
	#	${CMAKE_SOURCE_DIR}/game_shared/voice_status.cpp)
endif()

add_library (${CLDLL_LIBRARY} SHARED ${CLDLL_SOURCES})

if (GOLDSOURCE_SUPPORT)
	target_link_libraries( ${CLDLL_LIBRARY} ${CMAKE_DL_LIBS} )
endif()

if (USE_VGUI)
	if (WIN32)
		add_library(vgui SHARED IMPORTED)
		set_property(TARGET vgui PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/vgui_support/vgui-dev/lib/win32_vc6/vgui.dll")
		set_property(TARGET vgui PROPERTY IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/vgui_support/vgui-dev/lib/win32_vc6/vgui.lib")
		target_link_libraries(${CLDLL_LIBRARY} vgui)
	elseif(APPLE)
		target_link_libraries(${CLDLL_LIBRARY} "-Wl,--no-undefined -L${CMAKE_SOURCE_DIR}/vgui_support/vgui-dev/lib vgui.dylib")
	else()
		target_link_libraries(${CLDLL_LIBRARY} :vgui.so)
	endif()
endif()

if(WIN32)
	target_link_libraries( ${CLDLL_LIBRARY} user32.lib )
	if (GOLDSOURCE_SUPPORT)
		target_link_libraries( ${CLDLL_LIBRARY} winmm.lib )
	endif()
endif()

set_target_properties (${CLDLL_LIBRARY} PROPERTIES
	POSITION_INDEPENDENT_CODE 1)

if(BUILD_DISCORD_RPC)
	target_link_libraries(client discord-rpc)
endif()

if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Android")
	set(CLDLL_NAME "client")
	set_target_properties(${CLDLL_LIBRARY} PROPERTIES
		OUTPUT_NAME ${CLDLL_NAME}
		PREFIX "")
endif()

install( TARGETS ${CLDLL_LIBRARY}
	DESTINATION "${GAMEDIR}/${CLIENT_INSTALL_DIR}/"
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
	    GROUP_READ GROUP_EXECUTE
		WORLD_READ WORLD_EXECUTE )
