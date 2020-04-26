#!/bin/sh

day() {
    mult=$(($(date '+%d') %10))
    ending="th"

    case mult in
        1) ending="st" ;;
        2) ending="nd" ;;
        3) ending="rd" ;;
    esac

    printf "$(date '+%A, %_d')$ending $(date '+%B %Y')"
}

day
