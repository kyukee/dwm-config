#!/bin/sh

battery() {
    BAT="$(cat /sys/class/power_supply/BAT0/capacity)"

    if [ "$BAT" -lt 20 ]; then
        printf " %s" "$BAT"
    elif [ "$BAT" -lt 40 ]; then
        printf " %s" "$BAT"
    elif [ "$BAT" -lt 50 ]; then
        printf " %s" "$BAT"
    elif [ "$BAT" -lt 80 ]; then
        printf " %s" "$BAT"
    elif [ "$BAT" -lt 100 ]; then
        printf " %s" "$BAT"
    else
        printf " %s" "$BAT"
    fi
}

battery



# status="$(cat /sys/class/power_supply/AC/online)"
# battery="$(cat /sys/class/power_supply/BAT0/capacity)"

# case $status in
#     1) i='' ;;
#     0) case ${battery%?} in
#         [6-9]) i='' ;;
#         [1-5]) i='' ;;
#             *) i=''
#        esac
# esac
# printf "%s%%\n" "^c#d79921^$i^d^ $battery"
