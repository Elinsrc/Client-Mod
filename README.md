# Client-Mod

* **Client-Mod** is a small modification of the **Half-Life** client part for **Xash3D FWGS and GoldSource**

Added server name to no vgui scoreboard, player model name (included in vgui), curent map name and number of current players.

Added commands:

```
cl_autojump [0/1] - Will jump automatically as soon as you touch the ground while +jump is held. Taken from OpenAG.
```
* **When this command is enabled the jump button may not work**, solved by command **cl_lw 1**.
```
+ducktap — Performs a duck tap automatically as soon as you touch the ground (presses +duck for 1 frame). Taken from OpenAG.

hud_color "r g b" - Sets hud color, values must be enclosed in quotation marks. example: hud_color "255 0 255". Taken from OpenAG.

hud_weapon [0/1] - Draw current weapon in HUD, default value is 1. Taken from OpenAG.

cl_logchat [0/1/2 ] - Value 1 logs only messages from players. Value 2 logs system messages and players, default value is 0.

cl_chatsound [0/1/2] - Value 1 play system and players chat notification sound, Value 2 logs only from players, default value is 1.

cl_chatsound_path ["path"] - Path to chat notification playing sound, defaut path "misc/talk.wav".

hud_vis [0/1] - The color of armor and health indicators will change depending on their number, default value is 0.
```
* Comands starting with **vis_health** and **vis_battery** work with **hud_vis 1**. They change the color of the health indicator and change depending on their number.
* Example: **vis_health60 "0 255 0"** is the green color of health when it is greater than 60, the same with **vis_battery**.
```
vis_battery100 "r g b"
vis_battery80 "r g b"
vis_battery60 "r g b"
vis_battery40 "r g b"
vis_battery20 "r g b"

vis_health100 "r g b"
vis_health60 "r g b"
vis_health80 "r g b"
vis_health40 "r g b"
vis_health20 "r g b"

cl_weaponlowering [0/1] - The weapon in the player's hand will lower depending on the speed of movement, default value is 1.
cl_weaponsway [0/1] - Swaying of the weapon when moving, default value is 1.
cl_weaponlag [0/1] - Weapon lag when turnings, default value is 1.

hud_speedometer [0/1] - Shows the player's speed, default value is 0. Taken from OpenAG.
hud_speedometer_below_cross [0/1] - Adjusts the position of the speedometer to the sight, defaut value is 0.

hud_jumpspeed [0/1] - Shows the player's speed at the moment of the jump, default value is 0. Taken from OpenAG.
hud_jumpspeed_below_cross [0/1] - Adjusts the position of the jumpspeed to the sight, defaut value is 0.
hud_jumpspeed_height [0/1] - Adjusts the positio jumpspeed, defaut value is 0.

hud_strafeguide [0/1] - Shows strafeguide, default value is 0. Taken from OpenAG.
hud_strafeguide_zoom [0/1], defaut value is 1.
hud_strafeguide_height [value], defaut value 0.
hud_strafeguide_size [value], defaut value 0.

hud_watermark [0/1] - Shows client author and build date, defaut value is 1.

hud_rainbow [0/1] - Paints HUD rainbow, default value is 0. Taken from OpenAG.
hud_rainbow_sat [value], default value is 100.
hud_rainbow_val [value], default value is 100.
hud_rainbow_speed [value], default value is 40.
hud_rainbow_xphase [value], default value is 0.4.
hud_rainbow_yphase [value], default value is 0.7.

hud_deathnotice_bg [0/1] - Death notification background, default value is 0.

cl_gauss_balls [0/1] - Small bouncing balls from Gauss hits, defaut value is 1.
cl_gauss_hits [0/1] - A large glowing sprite from a Gauss hits, defaut value is 1.
cl_hidecorpses [0/1] - Hides the corpses of players. defaut value is 0.

cl_killsound [0/1] - Sound plays when you kill player, defaut value is 1. Taken from OpenAG.
cl_killsound_path ["path"] - Path to playing sound, defaut path "buttons/bell1.wav". Taken from OpenAG.

cl_cross [0/1] - Custom crosshair, defaut value 0.
cl_cross_color "r g b" - Custom crosshair color, defaut value "255 255 255".
cl_cross_alpha [value] - Crosshair alpha, defaut value 255.
cl_cross_size [value] - Crosshair size, defaut value 1.
cl_cross_gap [value] - Crosshair distance from the middle, defaut value 2.
cl_cross_thickness [value] - Thickness crosshair, defaut value 5.
cl_cross_outline [value] - Crosshairs outline defaut value 1.
cl_cross_top_line [0/1], defaut value 1.
cl_cross_bottom_line [0/1], defaut value 1.
cl_cross_left_line [0/1], defaut value 1.
cl_cross_right_line [0/1], defaut value 1.
cl_cross_dot_size [value], value 0 diable dot, defaut value 1.
cl_cross_dot_color "r g b", defaut value "255 255 255".

cl_viewmodel_ofs_right [value] — Offsets the gun viewmodel to the right (or to the left if cl_righthand is set to 1), 0 for no offset. Taken from OpenAG.
cl_viewmodel_ofs_forward [value] — Offsets the gun viewmodel forward, 0 for no offset. Taken from OpenAG.
cl_viewmodel_ofs_up [value] — Offsets the gun viewmodel upwards, 0 for no offset. Taken from OpenAG.

cl_debug [0/1/2] - Show client info, defaut value 0.
cl_debug_showfps [1/0] - Show curent fps and framerate, defaut value 1.
```

# [Build Instructions](BUILD.md)
