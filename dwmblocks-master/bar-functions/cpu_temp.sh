#!/bin/sh

calc_temp() {
    cpu_temp=$(< /sys/class/thermal/thermal_zone0/temp)
    cpu_temp=$(($cpu_temp/1000))
    printf "^c#b778f5^\uf2c8^d^ $cpu_temp°C"
}

calc_temp
