#!/bin/sh

battery_notif() {
    battery_level=`acpi -b | grep -P -o '[0-9]+(?=%)'`
    charging="$(cat /sys/class/power_supply/AC/online)"

    if [ $battery_level -ge 90 ] && [ "$charging" -eq 1 ]; then
        notify-send "Battery Full" "Level: ${battery_level}%"
        paplay --volume=50000 /usr/share/sounds/freedesktop/stereo/window-attention.oga
    elif [ $battery_level -le 30 ] && [ "$charging" -eq 0 ]; then
        notify-send --urgency=CRITICAL "Battery Low" "Level: ${battery_level}%"
        paplay --volume=50000 /usr/share/sounds/freedesktop/stereo/window-attention.oga
    fi
}

battery_notif
