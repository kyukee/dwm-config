#!/bin/sh

battery() {
    charge="$(cat /sys/class/power_supply/AC/online)"
    battery="$(cat /sys/class/power_supply/BAT0/capacity)"
    icon=""
    charge_icon=""

    if [ "$battery" -lt 20 ]; then
        icon=""
        charge_icon=" ^c#ba3222^\uea6c^d^"
    elif [ "$battery" -lt 40 ]; then
        icon=""
        charge_icon=" ^c#c2a30c^\uf06a^d^"
    elif [ "$battery" -lt 50 ]; then
        icon=""
    elif [ "$battery" -lt 80 ]; then
        icon=""
    elif [ "$battery" -lt 100 ]; then
        icon=""
    else
        icon=""
    fi

    if [ "$charge" -eq 1 ]; then
        charge_icon=" "
    fi

    printf "^c#b778f5^$icon^d^ $battery$charge_icon"
}

battery
