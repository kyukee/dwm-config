#!/bin/sh

cpu_load() {
    printf "$(top -bn1 | grep "Cpu(s)" | \
        sed "s/.*, *\([0-9.]*\)%* id.*/\1/" | \
        awk '{printf "%04.1f ", 100 - $1}')"

    # add leading (the second parameter is the total length after padding)
    # printf $(echo "$LOAD 4" | awk '{ for(c = 0; c < $2; c++) s = s"0"; s = s$1; print substr(s, 1 + length(s) - $2); }')

}

cpu_load
