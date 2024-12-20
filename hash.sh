#!/usr/bin/env bash
if [ -n "$1" ] && [ -f "$1" ] ; then
    fhash="$(basename $1)"
    fdir="$(dirname $1)"
    echo "Hashing MD5 of $1 to $fdir/$fhash.md5..."
    md5sum -b "$1" > "$fdir/$fhash.md5" 2>&1
    echo "Hashing SHA1 of $1 to $fdir/$fhash.sha1..."
    sha1sum -b "$1" > "$fdir/$fhash.sha1" 2>&1
    echo "Hashing SHA256 of $1 to $fdir/$fhash.sha256..."
    sha256sum -b "$1" > "$fdir/$fhash.sha256" 2>&1
    echo "Hashing BLAKE2 of $1 to $fdir/$fhash.blake2..."
    b2sum -b "$1" > "$fdir/$fhash.blake2" 2>&1
    return 0 2>/dev/null
    exit 0
else
    echo "Nothing to hash"
    return 1 2>/dev/null
    exit 1
fi
