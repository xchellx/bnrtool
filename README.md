# bnrtool
Encodes and decodes BNR banner files for the Nintendo GameCube.

## Table of Contents
- [Info](#info)
- [Building](#building)

## Info
Nintendo GameCube uses a banner file to display am icon, name, maker, and description at the disc section in the BIOS. A BNR file is custom binary which holds a magic number to identify whether it's a BNR1 or BNR 2 file, a 96x32 GX encoded RGB5A3 image, and text info of a short title and maker, a long title and maker, and a comment. These blocks of info can be of 6 different blocks for English, German, French, Spanish, Italian, and Dutch in BNR2 where in BNR1 it is just a single block for either English or Japanese. bnrtool can decode a BNR file to a PNG or TGA image and a YAML text file and back to a BNR file. It supports both BNR1 and BNR2 formats as well as ShiftJIS encoding for Japanese BNR1 files.

## Building

### Linux

#### Prerequisites
1. Install dependencies: `git p7zip cmake ninja clang` via your package manager.

#### dotnet CLI
2. `cd /path/to/directory/of/bnrtool_publish.sln`.
3. `dotnet publish bnrtool_publish.sln -p:PublishProfile=linux-x64`. You will get an error message of the build failing because for some reason dotnet CLI does not copy over files of `ProjectReference`s on the first build. Run the same command a second time for the build to succeed.
4. The resulting binary and it's dependencies should be in the 7z archive at `$(ProjectDir)/bin/x64/Release/<target_framework>/publish/linux-x64//bnrtool_<commit_or_tag>_msys2-clang64_$(TargetFramework)_linux-x64.7z`.

### Windows

#### Prerequisites
1. Go to [MSYS2's website](https://www.msys2.org) and follow directions `1-4` on the website and uncheck `Run MSYS2 now` at step 4 on the website.
2. Press the `Windows Key` (or click the Windows icon) and search for `msys2 clang64`, click the result titled `MSYS2 CLANG64`.
3. Update the package database: `pacman -Syu`. Be patient. If prompted, respond with `Y`. If the prompt is `:: To complete this update all MSYS2 processes including this terminal will be closed. Confirm to proceed [Y/n]`, then respond with `Y` which will close the window then repeat steps `2` and `3`.
4. Install dependencies: `pacman -S mingw-w64-clang-x86_64-cmake mingw-w64-clang-x86_64-ninja mingw-w64-clang-x86_64-clang`.

#### Visual Studio
1. Open `bnrtool.sln` in your file explorer (or within Visual Studio).
2. Right click `bnrtool` in the `Solution Explorer` panel and click `Publish`.
3. Click the `<rid>.pubxml` dropdown and select the `<rid>` of the platform you want to publish for`.
4. The resulting binary and it's dependencies should be in the 7z archive at `$(ProjectDir)/bin/x64/Release/<target_framework>/publish/win-x64//bnrtool_<commit_or_tag>_msys2-clang64_$(TargetFramework)_win-x64.7z`.

#### dotnet CLI
1. Open `cmd.exe`.
2. `cd ...:\path\to\directory\of\bnrtool_publish.sln`.
3. `dotnet publish bnrtool_publish.sln -p:PublishProfile=win-x64`. You will get an error message of the build failing because for some reason dotnet CLI does not copy over files of `ProjectReference`s on the first build. Run the same command a second time for the build to succeed.
4. The resulting binary and it's dependencies should be in the 7z archive at `$(ProjectDir)/bin/x64/Release/<target_framework>/publish/win-x64//bnrtool_<commit_or_tag>_msys2-clang64_$(TargetFramework)_win-x64.7z`.

### Additional
Although you can build/publish for Linux when on Windows (and vice versa), it is not supported due to how the build script works in GXBaseSharp/GXTextureSharp as it calls gxtexture_base_build/build.sh on Linux and gxtexture_base_build/build.cmd on Windows. As a result, if you target Linux when on Windows (and vice versa) it will just hang. Additionally, bnrtool builds with NativeAOT, which does not support cross platform building which means you couldn't even publish bnrtool for Linux when on Windows (and vice versa) in the first place.
