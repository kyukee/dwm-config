#!/bin/sh

cpu_load() {
    printf "\uec2f $(
        top -bn1 | grep "Cpu(s)" | \
        sed "s/.*, *\([0-9.]*\)%* id.*/\1/" | \
        # awk '{printf "%.1f ", 100 - $1; }' | \
        awk '{n= 100 - $1; s=" "n; s2= substr(s, length(s) - 3); if (length(s2) == 2) s2=s2".0"; print s2 }'
    )"

    # add leading spaces (the second parameter is the total length after padding)
    # echo "$LOAD 4" | awk '{ for(c = 0; c < $2; c++) s = s" "; s = s$1; print substr(s, 1 + length(s) - $2); }'
}

cpu_load
