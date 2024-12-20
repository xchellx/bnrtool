# bnrtool
Encodes and decodes BNR banner files for the Nintendo GameCube.

## Table of Contents
- [Info](#info)
- [Building](#building)
- [Credits](#credits)

## Info
Nintendo GameCube uses a banner file to display am icon, name, maker, and description at the disc section in the BIOS. A BNR file is custom binary which holds a magic number to identify whether it's a BNR1 or BNR 2 file, a 96x32 GX encoded RGB5A3 image, and text info of a short title and maker, a long title and maker, and a comment. These blocks of info can be of 6 different blocks for English, German, French, Spanish, Italian, and Dutch in BNR2 where in BNR1 it is just a single block for either English or Japanese. bnrtool can decode a BNR file to a PNG or TGA image and a YAML text file and back to a BNR file. It supports both BNR1 and BNR2 formats as well as ShiftJIS encoding for Japanese BNR1 files.

## Building

### Linux
TODO: Building.

### Windows
TODO: Building.

### Additional
Although you can build/publish for Linux on Windows, it is not supported due to how the build script works in GXBase/GXTextureSharp as it calls gxtexture_base_build/build.sh on Linux and gxtexture_base_build/build.cmd on Windows so if you are targetting Linux on Windows it will just hang. Additionally, bnrtool builds with NativeAOT, which does not support cross platform building, so you couldn't even publish bnrtool for Linux when on Windows in the first place.
