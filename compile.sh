#!/usr/bin/env bash
ASAN=""
OUTEXT=""
NCURSES="-lncurses"
if [ $# -gt 0 ]; then
    if [ "$1" == "ASAN" ]; then
        ASAN="-fsanitize=address -g"
    fi
fi
if [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ] || [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    OUTEXT=".exe"
    NCURSES=""
fi
gcc -std=c99 $NCURSES $ASAN -Wno-address-of-packed-member -Wno-stringop-overflow -Wvla -o bnrtool$OUTEXT bnrtool.c tga.c gxtexture.c strsep.c cgetchar.c
