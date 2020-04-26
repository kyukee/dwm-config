#!/bin/sh

calc_temp() {
    cpu_temp=$(< /sys/class/thermal/thermal_zone0/temp)
    cpu_temp=$(($cpu_temp/1000))
    printf "\uf2c8 $cpu_temp°C"
}

calc_temp
