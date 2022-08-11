# bnrtool
bnrtool is a tool that can extract and create Nintendo GameCube Banner (.bnr) files.

## Table of Contents
- [Info](#info)
- [Building](#building)
- [Credits](#credits)
- [License](#license)

## Info
Nintendo GameCube Banner files (.bnr) represent the image and text you'd see when the disc side of the cube menu. They
are in every disc as a file entry "opening.bnr", "peningUS.bnr", "openingEU.bnr", and/or "openingJP.bnr".

bnrtool can extract and create these files.

This tool has two modes: `export` and `import`.
In `export` mode, it can extract a BNR file to a TGA image and TXT document.
In `import` mode, it can create a BNR file from a TGA image and TXT document.

bnrtool does not handle GameCube game disc files, it only works on BNR files alone. You can use Dolphin (you can get it
[here](https://dolphin-emu.org/download/)) to extract the BNR file itself to work on it. For repacking the game disc
file after modifying the BNR, there are many tools but I recommend nodtool which you can find
[here](https://github.com/AxioDL/nod).

bnrtool extracts the image into TGA image in "Original TGA Format" with 32bit BGRA colors with 1 bit alpha and no
coordinate flipping bits set aswell as a width and height of 96 by 32. It expects this in `import` mode aswell. If you
use a "New TGA Format" TGA for `import` mode, the footer in this format will be ignored.

bnrtool extracts the comment(s) into a TXT document split into blocks of 5 lines. For BNR1, it's 1 block. For BNR2,
it's 6 blocks. In BNR1, the block can be of any language. For BNR2, each block represents a language in this order:
Comment #1 - English, Comment #2 - German, Comment #3 - French, Comment #4 - Spanish, Comment #5 - Italian, and Comment
\#6 - Dutch. For both BNR1 and BNR2, comment blocks consists of 5 lines each. The first line is the game title in short
form (32 characters max, single line). The second line is the game maker (company) in short form (32 characters max,
single line). The third line is the game title in long form (64 characters max, single line). The fourth line is the
game maker (company) in long form (64 characters max, single line). Finally, the fifth line is the game comment (
description) (128 characters max, 2 lines max). Empty lines are accepted.

## Building
Simply run `.\compile.sh` at root directory of project. This script uses GCC to compile the sources. It also computes
comthe md5, sha1, and sha256 hash of the compiled result afterwards using `md5sum`, `sha1sum`, and `sha256sum`. You
can pass `asan` as the first argument to the script to build a debug build with AddressSanitizer (this isn't supported
on a Windows build). For building for Windows, run this script with MYS2's MinGW x64 or run `./compile.bat`.

## Credits
- Custom Mario Kart Wiiki for it's documentation of the image formats of Nintendo GameCube's GX graphics system
[here](https://wiki.tockdom.com/wiki/Image_Formats#RGB5A3).
- Truevision Inc.'s documentation on the Targa (TGA) image format
[here](https://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf).
- FileFormat's documentation on the Targe (TGA) image format [here](http://www.paulbourke.net/dataformats/tga/)
- Paul Bourke's documentation on the Targe (TGA) image format [here](https://docs.fileformat.com/image/tga/)
- Dan Cross's implementation of strsep [here](https://unixpapa.com/incnote/string.html).
*"All of the C code on this page is public domain and may be used without concern for licenses."*.
- Yet Another Gamecube Documentation for it's documentation of BNR
[here](http://hitmen.c02.at/files/yagcd/yagcd/chap14.html)
- SweetScape's 010 Editor for BNR format research [here](https://www.sweetscape.com/010editor/)

## License
```
MIT License

Copyright (c) 2022 Yonder

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

```
