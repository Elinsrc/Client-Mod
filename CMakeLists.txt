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

# 3.9 added captures in if(MATCHES)
cmake_minimum_required(VERSION 3.9)

if(NOT ${CMAKE_VERSION} VERSION_LESS "3.15.0")
	cmake_policy(SET CMP0091 NEW)
endif()

# Install custom module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

include(CheckIncludeFile)
include(CheckCSourceCompiles)
include(VSForceXPToolchain) # Force XP toolchain for Visual Studio

project (CLIENT-MOD)

#--------------
# USER DEFINES \
################\
option(USE_VGUI "Enable VGUI1." OFF)
option(USE_NOVGUI_MOTD "Prefer non-VGUI MOTD when USE_VGUI is enabled" OFF)
option(USE_NOVGUI_SCOREBOARD "Prefer non-VGUI Scoreboard when USE_VGUI is enabled" OFF)
option(USE_DISCORD_RPC "Enable Discord_rpc" OFF)

if (CMAKE_SIZEOF_VOID_P EQUAL 4 OR
	((WIN32 OR ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	AND (CMAKE_SYSTEM_PROCESSOR STREQUAL "x64"
	OR CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64"
	OR CMAKE_SYSTEM_PROCESSOR STREQUAL "amd64")))
	option(64BIT "Disable auto -m32 appending to compiler flags" OFF)
	option(GOLDSOURCE_SUPPORT "Build goldsource compatible client library" ON)
else()
	option(64BIT "Disable auto -m32 appending to compiler flags" ON)
	option(GOLDSOURCE_SUPPORT "Build goldsource compatible client library" OFF)
endif()

#-----------------
# MAIN BUILD CODE \
###################\

if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND NOT 64BIT)
	if(MSVC)
		error("UNDONE: set 32 build flags")
	else()
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
	endif()
	set(CMAKE_SIZEOF_VOID_P 4)
endif()

if(64BIT AND CMAKE_SIZEOF_VOID_P EQUAL 4)
	message(FATAL_ERROR "You enabled XASH_64BIT, but compiler can't create 64 bit code!")
endif()

# Xash3D FWGS Library Naming Scheme compliance
# see documentation: https://github.com/FWGS/xash3d-fwgs/blob/master/Documentation/extensions/library-naming.md
include(LibraryNaming)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	message(STATUS "Building for 64 Bit")
else()
	message(STATUS "Building for 32 Bit")
endif()

# get current git commit short hash
execute_process(COMMAND "git" "describe" "--always" "--dirty" "--abbrev=7"
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE SHORT_HASH
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(SHORT_HASH)
	message(STATUS "Current Git commit hash: ${SHORT_HASH}")
	add_definitions(-DXASH_BUILD_COMMIT="${SHORT_HASH}")
else()
	message(STATUS "Failed to set current Git commit hash")
endif()

if (MINGW)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--add-stdcall-alias")
endif()

if(NOT MSVC)
	#add_compile_options(-Wempty-body) # GCC/Clang flag
	add_compile_options(-Wreturn-type) # GCC/Clang flag
else()
	add_definitions(-D_CRT_SILENCE_NONCONFORMING_TGMATH_H)
endif()

if(VITA)
	add_compile_options(-fno-use-cxa-atexit)
endif()

check_include_file("tgmath.h" HAVE_TGMATH_H)
if(HAVE_TGMATH_H)
	if(NOT MSVC)
		set(CMAKE_REQUIRED_LIBRARIES "m")
	endif()
	check_c_source_compiles("#include <tgmath.h>
	const float val = 2, val2 = 3;
	int main(){ return (int)(-asin(val) + cos(val2)); }" HAVE_VALID_TGMATH_H )
	if(${HAVE_VALID_TGMATH_H})
		add_definitions(-DHAVE_TGMATH_H)
	endif()
endif()

add_subdirectory(cl_dll)

if(USE_DISCORD_RPC)
	message(STATUS "Building discord_rpc enabled")
	set(BUILD_EXAMPLES OFF CACHE BOOL "build discord-rpc examples" FORCE)
	set(CLANG_FORMAT_SUFFIX "-dummy")
	add_subdirectory(3rdparty/discord-rpc)
endif()
