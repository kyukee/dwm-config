#!/bin/sh

cpu_load() {
    printf "^c#f76895^\uec2f^d^ $(
        top -bn1 | grep "Cpu(s)" | \
        sed "s/.*, *\([0-9.]*\)%* id.*/\1/" | \
        awk '{n= 100 - $1; s=""+n; l = length(s); if (l == 1) s=" "n".0"; else if (l == 2) s=n".0"; else if (l == 3) s=" "n; print s }'
    )"
}

cpu_load
