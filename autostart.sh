#!/bin/bash

## exec_always
feh --bg-fill /home/kyukee/Pictures/wallpapers/imageedit_8_8640256032.png
setxkbmap pt
xrdb ~/.Xresources
#/home/kyukee/Scripts/i3_dynamic_window_titles.sh

picom -b
xflux -l 38.7:-9.1 -k 4500 &
http-server ./.startpage -p 9000 &
mpd &
dunst -c ~/.config/dunst/dunstrc &
#nm-applet
#blueberry-tray
#volumeicon
#volti
#yad --notification --command='gsimplecal' --image='/usr/share/icons/clarity-albus/16x16/apps/calendar.png' --text='yad notification - Calendar'
nice -n 19 /home/kyukee/Scripts/fmenu-rofi.sh -u &
./dwmbar.sh &
(sleep 1; urxvt) &
