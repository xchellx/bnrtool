#!/usr/bin/env bash
dp0="$(dirname $(readlink -m $BASH_SOURCE))"
destdir="$(printf '%s' $1 | sed -E -e 's/\\/\//g' -e 's/C:/\/c/g' -e 's/\"*([^\"]+)\"*/\1/')"
iswin=""
[ -n "${4}" ] && iswin="1"
isfile=""
if [ -n "${4}" ] && [ "${4}" == *dll ] ; then
    isfile="1"
fi
[ -n "${iswin}" ] && lext=".dll" || lext=".so"
[ -n "${iswin}" ] && eext=".exe" || eext=""
echo "Packaging from publish dir ${destdir}..."
if [ -d "${destdir}" ] && [ -f "${destdir}/bnrtool${eext}" ] && ( [ -z "${isfile}" ] || [ -f "${destdir}/${4}" ] ) && [ -f "${destdir}/libgxtexture${lext}" ] ; then
        mkdir -p "${destdir}/package/bnrtool"
        mkdir -p "${destdir}/package/bnrtool/licenses"
        cp -f "${destdir}/bnrtool${eext}" "${destdir}/package/bnrtool/bnrtool${eext}"
        [ -n "${isfile}" ] && cp -f "${destdir}/${4}" "${destdir}/package/bnrtool/${4}"
        cp -f "${destdir}/libgxtexture${lext}" "${destdir}/package/bnrtool/libgxtexture${lext}"
        "$dp0/hash.sh" "${destdir}/package/bnrtool/bnrtool${eext}"
        [ -n "${isfile}" ] && "$dp0/hash.sh" "${destdir}/package/bnrtool/${4}"
        "$dp0/hash.sh" "${destdir}/package/bnrtool/libgxtexture${lext}"
        [ -n "${iswin}" ] && "$dp0/libcver.sh" "${destdir}/package/bnrtool"
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
    [ -f "${destdir}/bnrtool${eext}" ] || echo "${destdir}/bnrtool${eext} doesnt exist; packaging failed"
    if [ -n "${isfile}" ] && [ -f "${destdir}/${4}" ] ; then
        echo "${destdir}/${4} doesnt exist; packaging failed"
    fi
    [ -f "${destdir}/libgxtexture${lext}" ] || echo "${destdir}/libgxtexture${lext} doesnt exist; packaging failed"
    [ -d "${destdir}" ] || echo "${destdir} doesnt exist; packaging failed"
    return 1 2>/dev/null
    exit 1
fi

