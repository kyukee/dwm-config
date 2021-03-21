#!/bin/bash

cd $HOME

##### Executed every restart

feh --bg-fill /home/kyukee/Pictures/wallpapers/imageedit_8_8640256032.png
setxkbmap pt
xrdb ~/.Xresources
killall dwmblocks
dwmblocks &

##### Executed one time at startup

picom -b

if [ $? -ne 0 ]; then
    exit 0
fi

# tray icons
xflux -l 38.7:-9.1 -k 4500 &
mpd &
dunst -c ~/.config/dunst/dunstrc &
nm-applet &
parcittox &
blueman-tray &
volumeicon &
syncthing-gtk &
yad --notification --command='gsimplecal' --image='/usr/share/icons/clarity-albus/16x16/apps/calendar.png' --text='yad notification - Calendar' &

# wait and start a local server
~/.npm-global/lib/node_modules/http-server/bin/http-server ~/.startpage -p 9000 &

# update file index
nice -n 19 /home/kyukee/Scripts/fmenu-rofi.sh -u &

# wait and start a terminal
(sleep 1; kitty) &
