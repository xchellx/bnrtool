#!/usr/bin/env bash
dp0="$(dirname $(readlink -m $BASH_SOURCE))"
[ -n "$1" ] && [ -d "$1" ] && outdir="$1" || outdir="$dp0/build"
echo "Getting libc++.dll version and writing it to $outdir/libc++_version.txt..."
printf "#include <ciso646>\nint main () {}" | clang -E -stdlib=libc++ -x c++ -dM - | grep -E "^ *#define +_LIBCPP_VERSION" > "$outdir/libc++_version.txt"
