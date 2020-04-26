#!/bin/bash


##### Executed every restart

feh --bg-fill /home/kyukee/Pictures/wallpapers/imageedit_8_8640256032.png
setxkbmap pt
xrdb ~/.Xresources
#/home/kyukee/Scripts/i3_dynamic_window_titles.sh
killall dwmblocks
dwmblocks &

##### Executed one time at startup

picom -b

if [ $? -ne 0 ]; then
    exit 0
fi

# tray icons
xflux -l 38.7:-9.1 -k 4500 &
http-server ./.startpage -p 9000 &
mpd &
dunst -c ~/.config/dunst/dunstrc &
nm-applet &
blueberry-tray &
volumeicon &
yad --notification --command='gsimplecal' --image='/usr/share/icons/clarity-albus/16x16/apps/calendar.png' --text='yad notification - Calendar' &

nice -n 19 /home/kyukee/Scripts/fmenu-rofi.sh -u &
(sleep 1; urxvt) &
