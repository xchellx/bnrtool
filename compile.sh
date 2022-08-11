#!/usr/bin/env bash
ASAN=""
OUTEXT=""
if [ $# -gt 0 ]; then
    if [ "$1" == "ASAN" ]; then
        ASAN="-fsanitize=address -g"
    fi
fi
if [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ] || [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    OUTEXT=".exe"
fi
if [ ! -d "./dst" ]; then mkdir ./dst ; fi
gcc -std=c99 $ASAN -Wno-address-of-packed-member -Wno-stringop-overflow -Wvla -o ./dst/bnrtool$OUTEXT ./src/bnrtool.c ./src/tga.c ./src/gxtexture.c ./src/strsep.c ./src/cgetchar.c
md5sum -b ./dst/bnrtool$OUTEXT > ./dst/bnrtool$OUTEXT.md5
sha1sum -b ./dst/bnrtool$OUTEXT > ./dst/bnrtool$OUTEXT.sha1
sha256sum -b ./dst/bnrtool$OUTEXT > ./dst/bnrtool$OUTEXT.sha256
