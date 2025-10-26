# Client-Mod [![Build Status](https://github.com/Elinsrc/Client-Mod/actions/workflows/build.yml/badge.svg)](https://github.com/Elinsrc/Client-Mod/actions) <img align="right" src="https://raw.githubusercontent.com/Elinsrc/Client-Mod/refs/heads/master/android/app/src/main/res/drawable/ic_launcher.png" alt="Client-Mod" />

 - **Client-Mod** is a modification of the **Half-Life** client for **Xash3D FWGS** and **GoldSource**, based on [hlsdk-portable](https://github.com/FWGS/hlsdk-portable) and [PrimeXT](https://github.com/SNMetamorph/PrimeXT).

## Configuration (CVars)
| CVar                        | Default             | Min | Max | Description                                                                                                       |
|-----------------------------|---------------------|-----|-----|-------------------------------------------------------------------------------------------------------------------|
| cl_autojump                 | 1                   | 0   | 1   | Will jump automatically as soon as you touch the ground while +jump is held. Taken from OpenAG                    |
| +ducktap                    |                     | -   | -   | Performs a duck tap automatically as soon as you touch the ground (presses +duck for 1 frame). Taken from OpenAG  |
| hud_color                   | "255 160 0"         | -   | -   | Sets HUD color, values must be enclosed in quotation marks. Taken from OpenAG                                     |
| hud_weapon                  | 0                   | 0   | 1   | Draw current weapon in HUD. Taken from OpenAG                                                                     |
| vis_reload                  | 1                   | 0   | 1   | If the current weapon is displayed on the HUD, it will be colored while reloading                                 |
| vis_reload_color            | "250 250 250"       | -   | -   | The color the HUD will be painted when reloading                                                                  |
| cl_logchat                  | 0                   | 0   | 2   | Value 1 logs only messages from players. Value 2 logs system messages and players                                 |
| cl_chatsound                | 1                   | 0   | 2   | Value 1 plays system and player chat notification sound, Value 2 logs only from players                           |
| cl_chatsound_path           | "misc/talk.wav"     | -   | -   | Path to chat notification playing sound                                                                           |
| hud_vis                     | 0                   | 0   | 1   | The color of armor and health indicators will change depending on their number                                    |
| vis_battery100              | "0 250 0"           | -   | -   | Indicator color for 100% battery charge                                                                           |
| vis_battery80               | "0 250 0"           | -   | -   | Indicator color for 80% battery charge                                                                            |
| vis_battery60               | "250 250 0"         | -   | -   | Indicator color for 60% battery charge                                                                            |
| vis_battery40               | "250 100 0"         | -   | -   | Indicator color for 40% battery charge                                                                            |
| vis_battery20               | "250 0 0"           | -   | -   | Indicator color for 20% battery charge                                                                            |
| vis_health100               | "0 250 0"           | -   | -   | Indicator color for 100% health                                                                                   |
| vis_health80                | "0 250 0"           | -   | -   | Indicator color for 80% health                                                                                    |
| vis_health60                | "250 250 0"         | -   | -   | Indicator color for 60% health                                                                                    |
| vis_health40                | "250 100 0"         | -   | -   | Indicator color for 40% health                                                                                    |
| vis_health20                | "250 0 0"           | -   | -   | Indicator color for 20% health                                                                                    |
| cl_weaponlowering           | 1                   | 0   | 1   | The weapon in the player's hand will lower depending on the speed of movement                                     |
| cl_weaponsway               | 1                   | 0   | 1   | Swaying of the weapon when moving                                                                                 |
| cl_weaponlag                | 1                   | 0   | 1   | Weapon lag when turning                                                                                           |
| hud_speedometer             | 0                   | 0   | 1   | Shows the player speed. Taken from OpenAG                                                                         |
| hud_speedometer_below_cross | 0                   | 0   | 1   | Adjusts the position of the speedometer to the sight                                                              |
| hud_jumpspeed               | 0                   | 0   | 1   | Shows the player speed at the moment of the jump. Taken from OpenAG                                               |
| hud_jumpspeed_below_cross   | 0                   | 0   | 1   | Adjusts the position of the jumpspeed to the sight                                                                |
| hud_jumpspeed_height        | 0                   | -   | -   | Adjusts the position of jumpspeed                                                                                 |
| hud_strafeguide             | 0                   | 0   | 1   | Shows strafeguide. Taken from OpenAG                                                                              |
| hud_strafeguide_zoom        | 1                   | -   | -   |                                                                                                                   |
| hud_strafeguide_height      | 0                   | -   | -   |                                                                                                                   |
| hud_strafeguide_size        | 0                   | -   | -   |                                                                                                                   |
| hud_watermark               | 1                   | -   | -   | Shows client author and build date                                                                                |
| hud_rainbow                 | 1                   | 0   | 1   | Paints HUD rainbow. Taken from OpenAG                                                                             |
| hud_rainbow_sat             | 100                 | -   | -   |                                                                                                                   |
| hud_rainbow_val             | 100                 | -   | -   |                                                                                                                   |
| hud_rainbow_speed           | 40                  | -   | -   |                                                                                                                   |
| hud_rainbow_xphase          | 0.4                 | -   | -   |                                                                                                                   |
| hud_rainbow_yphase          | 0.7                 | -   | -   |                                                                                                                   |
| hud_deathnotice_bg          | 1                   | 0   | 1   | Death notification background                                                                                     |
| cl_gauss_balls              | 1                   | 0   | 1   | Small bouncing balls from Gauss hits                                                                              |
| cl_gauss_hits               | 1                   | 0   | 1   | A large glowing sprite from Gauss hits                                                                            |
| cl_hidecorpses              | 0                   | 0   | 1   | Hides the corpses of players                                                                                      |
| cl_killsound                | 1                   | -   | -   | Sound plays when you kill a player. Taken from OpenAG                                                             |
| cl_killsound_path           | "buttons/bell1.wav" | -   | -   | Path to playing sound. Taken from OpenAG.                                                                         |
| cl_cross                    | 0                   | 0   | 1   | Custom crosshair                                                                                                  |
| cl_cross_color              | "0 255 0"           | -   | -   | Custom crosshair color                                                                                            |
| cl_cross_alpha              | 255                 | -   | -   | Crosshair alpha                                                                                                   |
| cl_cross_size               | 10                  | -   | -   | Crosshair size                                                                                                    |
| cl_cross_gap                | 3                   | -   | -   | Crosshair distance from the middle                                                                                |
| cl_cross_thickness          | 2                   | -   | -   | Thickness of crosshair                                                                                            |
| cl_cross_outline            | 1                   | 0   | 1   | Crosshair outline                                                                                                 |
| cl_cross_top_line           | 1                   | 0   | 1   |                                                                                                                   |
| cl_cross_bottom_line        | 1                   | 0   | 1   |                                                                                                                   |
| cl_cross_left_line          | 1                   | 0   | 1   |                                                                                                                   |
| cl_cross_right_line         | 1                   | 0   | 1   |                                                                                                                   |
| cl_cross_dot_size           | 1                   | -   | -   | Value 0 disables the dot                                                                                          |
| cl_cross_dot_color          | "0 255 0"           | -   | -   | Custom crosshair dot color                                                                                        |
| cl_cross_circle_radius      | 0                   | -   | -   | Controls the radius of the circle drawn for the crosshair (works only if built with ImGui)                        |
| cl_cross_circle_gap         | 3                   | -   | -   | Controls the gap between the crosshair center and the circle (works only if built with ImGui)                     |
| cl_cross_circle_color       |"0 255 0"            | -   | -   | Sets the circle color (works only if built with ImGui)                                                            |
| cl_viewmodel_ofs_right      | 0                   | -   | -   | Offsets the gun viewmodel to the right. Taken from OpenAG                                                         |
| cl_viewmodel_ofs_forward    | 0                   | -   | -   | Offsets the gun viewmodel forward. Taken from OpenAG                                                              |
| cl_viewmodel_ofs_up         | 0                   | -   | -   | Offsets the gun viewmodel upwards. Taken from OpenAG                                                              |
| cl_debug                    | 0                   | 0   | 3   | Show client info                                                                                                  |
| cl_debug_showfps            | 1                   | 0   | 1   | Show current fps and framerate                                                                                    |
| hud_health_divider          | 1                   | 0   | 1   | Show separator bar after health                                                                                   |
| hud_allow_hd                | 0                   | 0   | 1   | Draw HL25 style HUD and disable hud_deathnotice_bg                                                                |
| ui_imgui_demo               | -                   | -   | -   | Shows the ImGui demo window                                                                                       |
| ui_imgui_scoreboard         | 0                   | 0   | 1   | Switches the scoreboard to the one implemented using ImGui                                                        |
| ui_imgui_scale              | 0                   | 1   | -   | Adjusts the scaling of the ImGui elements                                                                         |

## Building
Clone the source code:
```
git clone https://github.com/Elinsrc/Client-Mod --recursive
```
### Windows
```
cmake -A Win32 -S . -B build
cmake --build build --config Release
```
### Linux
```
cmake -S . -B build
cmake --build build --config Release
```
### Android
```
./gradlew assembleDebug
```

## Thanks
 - Thanks To Valve for [Half-Life](https://store.steampowered.com/app/70/HalfLife/) and [hlsdk](https://github.com/ValveSoftware/halflife)
 - Thanks To [FWGS](https://github.com/FWGS) for [hlsdk-portable](https://github.com/FWGS/hlsdk-portable)
 - Thanks To [YaLTeR](https://github.com/YaLTeR) for [OpenAG](https://github.com/YaLTeR/OpenAG)
 - Thanks To [Velaron](https://github.com/Velaron) for [AndroidLauncher](https://github.com/Elinsrc/Client-Mod/tree/master/android)
 - Thanks To [SNMetamorph](https://github.com/SNMetamorph) for [BuildInfo](https://github.com/Elinsrc/Client-Mod/blob/master/public/build_info.h), [goldsrc-monitor](https://github.com/SNMetamorph/goldsrc-monitor) and [imgui_manager](https://github.com/SNMetamorph/PrimeXT/tree/master/client/ui)
