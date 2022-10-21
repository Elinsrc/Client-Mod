# Client-Mod

* Client mod is a small modification of the "half life" client part for "Xash3D FWGS and GoldSource"
 
Added a new control with additional buttons to activate "Configuration->Touch->Touch options", select preset "New" and click Activate.

Added server name to scoreboard and current date with time.
 
Added commands:
  
cl_autojump - [1/0] Autojump on SPACEBAR key, default value is 1.
When this command is enabled the jump button may not work, solved by command cl_lw 1. Taken from OpenAG.
  
hud_color "r g b" - Sets hud color, values must be enclosed in quotation marks. example: hud_color "255 0 255".

hud_weapon - [1/0] Draw current weapon in HUD, default value is 1.

hud_new - [1/0] The color of armor and health indicators will change depending on their number, default value is 0.

Comands starting with hud_health and hud_battery work with hud_new 1. They change the color of the health indicator and change depending on their number. Example: hud_health60 "0 255 0" is the green color of health when it is greater than 60, the same with hud_battery.

  hud_battery100 "r g b"
  
  hud_battery60 "r g b"
  
  hud_battery40 "r g b"

  hud_battery20 "r g b"
  
  hud_health100 "r g b"

  hud_health60 "r g b"
 
  hud_health40 "r g b"
 
  hud_health20 "r g b"

cl_lowerweapon - [1/0] The weapon in the player's hand will lower depending on the speed of movement, default value is 1.

cl_weaponsway - [1/0] Swaying of the weapon when moving, default value is 1.

cl_weaponlag - [1/0] weapon lag when turning, default value is 1.

cl_gausscolor - [1/0] activates cvar cl_gaussbeam "r g b", default value is 0.

hud_speedometer - [1/0] shows the player's speed, default value is 1. 

hud_speedometer_below_cross - [1/0] adjusts the position of the speedometer to the sight, defaut value is 0.

cl_colored_speed - [1/0] activates cvar hud_speedcolor "r g b" - draw custom speedometer color.

hud_jumpspeed - [1/0] shows the player's speed at the moment of the jump, default value is 1. Taken from OpenAG.

hud_jumpspeed_below_cross - [1/0] adjusts the position of the jumpspeed to the sight, defaut value is 0.

hud_jumpspeed_height - [1/0] adjusts the positio jumpspeed, defaut value is 0.

cl_colored_jump - [1/0] activates cvar hud_jumpcolor "r g b" - draw custom jumpspeed color.

cl_logchat - [1/0 ] records the entire chat log during the game, the log is stored next to the "valve" folder, defaut value is 1.

cl_cross - [1/0] custom crosshair, defaut value 0.

cl_cross_color "r g b" - custom crosshair color, defaut value "255 255 255".

cl_cross_alpha [value] - crosshair alpha, defaut value 255.

cl_cross_size [value] - crosshair size, defaut value 1.

cl_cross_gap [value] - crosshair distance from the middle, defaut value 2.

cl_cross_thickness [value] - thickness crosshair, defaut value 5.

cl_cross_outline [value] - crosshairs outline defaut value 1.

cl_cross_top_line [1/0] ,defaut value 1.

cl_cross_bottom_line [1/0] ,defaut value 1.

cl_cross_left_line [1/0] ,defaut value 1.

cl_cross_right_line [1/0] ,defaut value 1.

cl_cross_dot [1/0] ,defaut value 1.

cl_cross_dot_size [value] ,defaut value 1.

cl_cross_dot_color "r g b" ,defaut value "255 255 255".

unfinished command cl_chatbg - [1/0],defaut value 0.

In the future, the list of commands will be updated and a android port for the new engine will appear.
