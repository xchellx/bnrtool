#!/usr/bin/env bash
dp0="$(dirname $(readlink -m $BASH_SOURCE))"
echo "Building with cmake into $dp0/build/txtrtool.exe..."
[ -d "$dp0/build" ] && "$dp0/delcfg.sh"
[ -d "$dp0/build" ] && rm -rf "$dp0/build"
cmake -S "$dp0" -B "$dp0/build" -DCMAKE_BUILD_TYPE=Release -G Ninja -DSTBIR_SSE2:BOOL=ON -DSTBIR_AVX:BOOL=ON -DSTBIR_AVX2:BOOL=ON -DBUILD_SQUISH_WITH_SSE2:BOOL=ON
if [ -d "$dp0/build" ] ; then
    cmake --build "$dp0/build"
    "$dp0/copylibs.sh" "$dp0/build"
    if [ -f "$dp0/build/txtrtool.exe" ] && [ -f "$dp0/build/libc++.dll" ] ; then
        mkdir -p "$dp0/build/package/txtrtool"
        mkdir -p "$dp0/build/package/txtrtool/licenses"
        cp -f "$dp0/build/txtrtool.exe" "$dp0/build/package/txtrtool/txtrtool.exe"
        cp -f "$dp0/build/libc++.dll" "$dp0/build/package/txtrtool/libc++.dll"
        "$dp0/hash.sh" "$dp0/build/package/txtrtool/txtrtool.exe"
        "$dp0/hash.sh" "$dp0/build/package/txtrtool/libc++.dll"
        "$dp0/libcver.sh" "$dp0/build/package/txtrtool"
        cp -f "$dp0/LICENSE" "$dp0/build/package/txtrtool/LICENSE"
        cp -f "$dp0/README.md" "$dp0/build/package/txtrtool/README.md"
        cp -f -R "$dp0/licenses/." "$dp0/build/package/txtrtool/licenses"
        cp -f -R "$dp0/extern/gxtexture_base/licenses/." "$dp0/build/package/txtrtool/licenses"
        VERSION="$(git describe --exact-match --tags 2> /dev/null)"
        if [ -z "${VERSION}" ] ; then 
            [ -z "$(git status --porcelain 2> /dev/null)" ] && VERSION="$(git rev-parse --short HEAD 2> /dev/null)" || VERSION="$(git rev-parse --short HEAD 2> /dev/null)-dirty"
        fi
        [ -z "${VERSION}" ] && VERSION="unknown"
        7za a -ssw -t7z -mx=9 -myx=9 -ms=off -mmt=16 -mmtf=on -mtm=off -mtc=off -mta=off -m0=LZMA2:d=256m:mf=bt4:lc=4:fb=256 "$dp0/build/txtrtool_${VERSION}_msys2-clang64_win-x64.7z" "$dp0/build/package/txtrtool/"
        return 0 2>/dev/null
        exit 0
    else
        echo "Build files dont exist; build failed"
        return 1 2>/dev/null
        exit 1
    fi
else
    echo "$dp0/build doesnt exist; build failed"
    return 1 2>/dev/null
    exit 1
fi

