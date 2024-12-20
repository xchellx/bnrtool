#!/usr/bin/env bash
dp0="$(dirname $(readlink -m $BASH_SOURCE))"
destdir="$(printf '%s' $1 | sed -E -e 's/\\/\//g' -e 's/C:/\/c/g' -e 's/\"*([^\"]+)\"*/\1/')"
echo "Packaging from publish dir ${destdir}..."
if [ -d "${destdir}" ] && [ -f "${destdir}/bnrtool.exe" ] && ( [ -n "${4}" ] || [ -f "${destdir}/libc++.dll" ] ) && [ -f "${destdir}/libgxtexture.dll" ] ; then
        mkdir -p "${destdir}/package/bnrtool"
        mkdir -p "${destdir}/package/bnrtool/licenses"
        cp -f "${destdir}/bnrtool.exe" "${destdir}/package/bnrtool/bnrtool.exe"
        [ -n "${4}" ] && cp -f "${destdir}/libc++.dll" "${destdir}/package/bnrtool/libc++.dll"
        cp -f "${destdir}/libgxtexture.dll" "${destdir}/package/bnrtool/libgxtexture.dll"
        "$dp0/hash.sh" "${destdir}/package/bnrtool/bnrtool.exe"
        [ -n "${4}" ] && "$dp0/hash.sh" "${destdir}/package/bnrtool/libc++.dll"
        "$dp0/hash.sh" "${destdir}/package/bnrtool/libgxtexture.dll"
        [ -n "${4}" ] && "$dp0/libcver.sh" "${destdir}/package/bnrtool"
        cp -f "$dp0/LICENSE" "${destdir}/package/bnrtool/LICENSE"
        cp -f "$dp0/README.md" "${destdir}/package/bnrtool/README.md"
        cp -f -R "$dp0/licenses/." "${destdir}/package/bnrtool/licenses"
        cp -f -R "$dp0/extern/GXBaseSharp/extern/gxtexture_base_build/extern/gxtexture_base/licenses/." "${destdir}/package/bnrtool/licenses"
        VERSION="$(git describe --exact-match --tags 2> /dev/null)"
        if [ -z "${VERSION}" ] ; then 
            [ -z "$(git status --porcelain 2> /dev/null)" ] && VERSION="$(git rev-parse --short HEAD 2> /dev/null)" || VERSION="$(git rev-parse --short HEAD 2> /dev/null)-dirty"
        fi
        [ -z "${VERSION}" ] && VERSION="unknown"
        7za a -ssw -t7z -mx=9 -myx=9 -ms=off -mmt=16 -mmtf=on -mtm=off -mtc=off -mta=off -m0=LZMA2:d=256m:mf=bt4:lc=4:fb=256 "${destdir}/bnrtool_${VERSION}_msys2-clang64_$2_$3.7z" "${destdir}/package/bnrtool/"
        return 0 2>/dev/null
        exit 0
else
    [ -f "${destdir}/bnrtool.exe" ] || echo "${destdir}/bnrtool.exe doesnt exist; packaging failed"
    if [ -n "${4}" ] && [ -f "${destdir}/libc++.dll" ] ; then
        echo "${destdir}/libc++.dll doesnt exist; packaging failed"
    fi
    [ -f "${destdir}/libgxtexture.dll" ] || echo "${destdir}/libgxtexture.dll doesnt exist; packaging failed"
    [ -d "${destdir}" ] || echo "${destdir} doesnt exist; packaging failed"
    return 1 2>/dev/null
    exit 1
fi

