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

cmake_minimum_required(VERSION 3.9)
project (CLDLL)

set (CLDLL_LIBRARY client)

set(GAMEDIR "valve" CACHE STRING "Gamedir path")
set(CLIENT_INSTALL_DIR "cl_dlls" CACHE STRING "Where to put client dlls")

if(NOT MSVC)
	add_compile_options(-fno-exceptions) # GCC/Clang flag
	add_compile_options(-fno-rtti) # GCC/Clang flag
	add_compile_options(-fvisibility=hidden) # GCC/Clang flag
	add_definitions(-D_LINUX -DLINUX) # It seems enough for all non-Win32 systems
	add_definitions(-Dstricmp=strcasecmp -Dstrnicmp=strncasecmp)
	if(NOT MINGW)
		add_definitions(-D_snprintf=snprintf -D_vsnprintf=vsnprintf)
	endif()
	if(CMAKE_BUILD_TYPE MATCHES "Release"
		OR (CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo"
		AND CMAKE_SYSTEM_NAME STREQUAL "Android"))
		add_compile_options(-fno-unwind-tables -fno-asynchronous-unwind-tables) # GCC/Clang flag
		add_compile_options(-fomit-frame-pointer) # GCC/Clang flag
		add_compile_options(-ftree-vectorize) # GCC/Clang flag
		add_compile_options(-funsafe-math-optimizations) # GCC/Clang flag
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

if(USE_DISCORD_RPC)
	add_definitions(-DUSE_DISCORD_RPC)
endif()

if(USE_IMGUI)
	add_definitions(-DUSE_IMGUI)
endif()

set (CLDLL_SOURCES
	${CMAKE_SOURCE_DIR}/dlls/wpn_shared/crossbow.cpp
	${CMAKE_SOURCE_DIR}/dlls/wpn_shared/crowbar.cpp
	${CMAKE_SOURCE_DIR}/dlls/wpn_shared/egon.cpp
	${CMAKE_SOURCE_DIR}/dlls/wpn_shared/gauss.cpp
	${CMAKE_SOURCE_DIR}/dlls/wpn_shared/handgrenade.cpp
	${CMAKE_SOURCE_DIR}/dlls/wpn_shared/hornetgun.cpp
	${CMAKE_SOURCE_DIR}/dlls/wpn_shared/mp5.cpp
	${CMAKE_SOURCE_DIR}/dlls/wpn_shared/python.cpp
	${CMAKE_SOURCE_DIR}/dlls/wpn_shared/rpg.cpp
	${CMAKE_SOURCE_DIR}/dlls/wpn_shared/satchel.cpp
	${CMAKE_SOURCE_DIR}/dlls/wpn_shared/shotgun.cpp
	${CMAKE_SOURCE_DIR}/dlls/wpn_shared/squeakgrenade.cpp
	${CMAKE_SOURCE_DIR}/dlls/wpn_shared/tripmine.cpp
	${CMAKE_SOURCE_DIR}/dlls/wpn_shared/glock.cpp
	events/ev_hldm.cpp
	events/ev_common.cpp
	events/events.cpp
	hl/hl_baseentity.cpp
	hl/hl_events.cpp
	hl/hl_objects.cpp
	hl/hl_weapons.cpp
	input/in_camera.cpp
	input/input.cpp
	input/input_goldsource.cpp
	input/input_mouse.cpp
	input/input_xash3d.cpp
	rendering/GameStudioModelRenderer.cpp
	rendering/StudioModelRenderer.cpp
	rendering/tri.cpp
	utils/custom_utils.cpp
	ui/voice_banmgr.cpp
	ui/hud/ammo.cpp
	ui/hud/ammo_secondary.cpp
	ui/hud/ammohistory.cpp
	ui/hud/battery.cpp
	ui/hud/death.cpp
	ui/hud/flashlight.cpp
	ui/hud/geiger.cpp
	ui/hud/health.cpp
	ui/hud/hud.cpp
	ui/hud/hud_msg.cpp
	ui/hud/hud_redraw.cpp
	ui/hud/hud_spectator.cpp
	ui/hud/hud_update.cpp
	ui/hud/hud_crosshairs.cpp
	ui/hud/hud_watermark.cpp
	ui/hud/hud_debug.cpp
	ui/hud/menu.cpp
	ui/hud/message.cpp
	ui/hud/saytext.cpp
	ui/hud/status_icons.cpp
	ui/hud/statusbar.cpp
	ui/hud/text_message.cpp
	ui/hud/train.cpp
	ui/hud/OpenAG/color_tags.cpp
	ui/hud/OpenAG/rainbow.cpp
	ui/hud/OpenAG/hud_strafeguide.cpp
	ui/hud/OpenAG/hud_speedometer.cpp
	ui/hud/OpenAG/hud_jumpspeed.cpp
	ui/hud/OpenAG/hud_countdown.cpp
	ui/hud/OpenAG/hud_settings.cpp
	ui/hud/OpenAG/hud_timer.cpp
	ui/hud/OpenAG/hud_ctf.cpp
	ui/hud/OpenAG/hud_customtimer.cpp
	ui/hud/OpenAG/hud_location.cpp
	ui/hud/OpenAG/hud_nextmap.cpp
	ui/hud/OpenAG/hud_playerid.cpp
	ui/hud/OpenAG/hud_suddendeath.cpp
	ui/hud/OpenAG/hud_timeout.cpp
	ui/hud/OpenAG/hud_vote.cpp
	# ui/hud/OpenAG/hud_splash.cpp
	ui/hud/OpenAG/hud_longjump.cpp
	cdll_int.cpp
	com_weapons.cpp
	demo.cpp
	entity.cpp
	parsemsg.cpp
	studio_util.cpp
	util.cpp
	view.cpp
	${CMAKE_SOURCE_DIR}/pm_shared/pm_debug.c
	${CMAKE_SOURCE_DIR}/pm_shared/pm_math.c
	${CMAKE_SOURCE_DIR}/pm_shared/pm_shared.c)

if (USE_VGUI)
	list(APPEND CLDLL_SOURCES
		ui/voice_status.cpp
		ui/VGUI/vgui_int.cpp
		ui/VGUI/vgui_ClassMenu.cpp
		ui/VGUI/vgui_ConsolePanel.cpp
		ui/VGUI/vgui_ControlConfigPanel.cpp
		ui/VGUI/vgui_CustomObjects.cpp
		ui/VGUI/vgui_MOTDWindow.cpp
		ui/VGUI/vgui_SchemeManager.cpp
		ui/VGUI/vgui_ScorePanel.cpp
		ui/VGUI/vgui_TeamFortressViewport.cpp
		ui/VGUI/vgui_SpectatorPanel.cpp
		ui/VGUI/vgui_teammenu.cpp
		ui/VGUI/utils/vgui_checkbutton2.cpp
		ui/VGUI/utils/vgui_grid.cpp
		ui/VGUI/utils/vgui_helpers.cpp
		ui/VGUI/utils/vgui_listbox.cpp
		ui/VGUI/utils/vgui_loadtga.cpp
		ui/VGUI/utils/vgui_scrollbar2.cpp
		ui/VGUI/utils/vgui_slider2.cpp
		)
	if (USE_NOVGUI_MOTD)
		list(APPEND CLDLL_SOURCES ui/hud/MOTD.cpp)
	endif()
	if (USE_NOVGUI_SCOREBOARD)
		list(APPEND CLDLL_SOURCES ui/hud/scoreboard.cpp)
	endif()
else()
	list(APPEND CLDLL_SOURCES
		ui/novgui_voice_status.cpp
		ui/hud/MOTD.cpp
		ui/hud/scoreboard.cpp)
endif()

if (USE_DISCORD_RPC)
	list(APPEND CLDLL_SOURCES
		discord-rpc/discord_integration.cpp)
endif()

if (USE_IMGUI)
	list(APPEND CLDLL_SOURCES
		ui/IMGUI/imgui_backend.cpp
		ui/IMGUI/imgui_manager.cpp
		ui/IMGUI/imgui_window_system.cpp
		ui/IMGUI/ui_main.cpp
		ui/IMGUI/ui_demo_window.cpp
		${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp
		${CMAKE_SOURCE_DIR}/external/imgui/imgui_demo.cpp
		${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp
		${CMAKE_SOURCE_DIR}/external/imgui/imgui_tables.cpp
		${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp)
endif()

if (USE_IMGUI AND ANDROID)
	list(APPEND CLDLL_SOURCES
	rendering/gl_export.cpp)
endif()

include_directories (. hl/ events/ input/ rendering/ ui ui/hud ui/hud/OpenAG/ utils/ ${CMAKE_SOURCE_DIR}/dlls ${CMAKE_SOURCE_DIR}/dlls/wpn_shared ${CMAKE_SOURCE_DIR}/common ${CMAKE_SOURCE_DIR}/engine ${CMAKE_SOURCE_DIR}/pm_shared ${CMAKE_SOURCE_DIR}/game_shared ${CMAKE_SOURCE_DIR}/public)

if (USE_DISCORD_RPC)
	include_directories(${CMAKE_SOURCE_DIR}/external/discord-rpc/include discord-rpc)
endif()

if (USE_VGUI)
	SET(CMAKE_SKIP_RPATH TRUE)
	link_directories(${CMAKE_SOURCE_DIR}/vgui_support/vgui-dev/lib)
	include_directories(ui/VGUI ui/VGUI/utils/ ${CMAKE_SOURCE_DIR}/vgui_support/vgui-dev/include)
else()
	include_directories(${CMAKE_SOURCE_DIR}/utils/fake_vgui/include)
endif()

if(USE_VOICEMGR)
	#	ui/VGUI/utils/voice_banmgr.cpp
	#	ui/VGUI/utils/voice_status.cpp)
endif()

if (USE_IMGUI)
	include_directories(${CMAKE_SOURCE_DIR}/external/imgui ui/IMGUI)
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
    target_link_libraries(${CLDLL_LIBRARY} user32.lib opengl32.lib)
    if (GOLDSOURCE_SUPPORT)
        target_link_libraries(${CLDLL_LIBRARY} winmm.lib)
    endif()
endif()

if(USE_DISCORD_RPC)
	target_link_libraries(${CLDLL_LIBRARY} discord-rpc)
endif()

set_target_properties (${CLDLL_LIBRARY} PROPERTIES
	POSITION_INDEPENDENT_CODE 1)

set_target_postfix(${CLDLL_LIBRARY})

set_property(TARGET ${CLDLL_LIBRARY} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if(HAVE_LTO)
	set_property(TARGET ${CLDLL_LIBRARY} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

install(TARGETS ${CLDLL_LIBRARY}
	RUNTIME DESTINATION "${GAMEDIR}/${CLIENT_INSTALL_DIR}/"
	LIBRARY DESTINATION "${GAMEDIR}/${CLIENT_INSTALL_DIR}/"
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_EXECUTE
		WORLD_READ WORLD_EXECUTE)

if(CMAKE_BUILD_TYPE MATCHES "Release" AND NOT WIN32)
	add_custom_command(TARGET ${CLDLL_LIBRARY}
			POST_BUILD DEPENDS ${CLDLL_LIBRARY}
			COMMAND ${CMAKE_STRIP} -s $<TARGET_FILE:${CLDLL_LIBRARY}>)
endif()
