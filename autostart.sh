#!/bin/bash

cd $HOME

##########################
# Executed every restart #
##########################

feh --bg-fill /home/kyukee/Pictures/wallpapers/wallpaper_symlink
xrdb ~/.Xresources
killall dwmblocks
dwmblocks &

# setxkbmap us -variant altgr-intl -option "shift:both_capslock,ctrl:nocaps"
# /etc/default/keyboard

################################
# Executed one time at startup #
################################

# start picom if not already running; detect dwm restart based on exit code
picom -b

if [ $? -ne 0 ]; then
    exit 0
fi

# ------------
#  tray icons
# ------------

nm-applet &
parcittox &
blueman-applet &
# blueberry-tray > /tmp/blueberry-tray-out.txt 2>&1 &
volctl &
yad --notification --command='gsimplecal' --image='/usr/share/icons/Suru++-Asprómauros/apps/16/calendar-app.svg' --text='yad notification - Calendar' &
caffeine &
fluxgui &
pass "Password Manager/keepass" | keepassxc --pw-stdin ~/Sync/Documents/Keepass/password_database.kdbx &
arandr-indicator &
syncthing-gtk &
jellyfin-mpv-shim &
kdeconnect-indicator &

# --------------------
#  background daemons
# --------------------

# color temperature
# xflux -l '38' -g '-9' -k 4500 &

# music
mpd &

# notifications
dunst -c ~/.config/dunst/dunstrc &

# window switcher
alttab -mk Super_L -w 1 -d 1 &

# battery level alert
/home/kyukee/Scripts/battery_level_alert.sh &

# local http server for custom startpage
~/.npm-global/lib/node_modules/http-server/bin/http-server ~/Development/git/startpage -p 9000 &

# flash window on focus change
flashfocus &

# -----------------
#  startup actions
# -----------------

# update file index
nice -n 19 /home/kyukee/Scripts/fmenu-rofi.sh -u &

# adjust monitor configuration
autorandr --change

# wait and start a terminal
(sleep 1; kitty) &
