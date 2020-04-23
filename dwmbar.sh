SEP=" | "

day() {
    echo -e "$(date '+%Y %b %d')"
}

clock() {
    echo -e "$(date '+%H:%M')"
}

battery() {
    echo -e "BAT $(cat /sys/class/power_supply/BAT0/capacity)"
}

while true; do
    xsetroot -name "$(battery);$(day)$SEP$(clock)"
    sleep 1
done
