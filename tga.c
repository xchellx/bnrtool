/*
 * MIT License
 * 
 * Copyright (c) 2022 Yonder
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "tga.h"

void TGA_WriteHeader(uint16_t width, uint16_t height, FILE* stream) {
    TGAHeader* tgaHeader = (TGAHeader*) malloc(sizeof(TGAHeader));
    
    tgaHeader->idLength = 0;
    // 0 = no color-map
    tgaHeader->colorMapType = 0;
    // 2 = Uncompressed True-color Image
    tgaHeader->imageType = 2;
    tgaHeader->firstEntryIndex = 0;
    tgaHeader->colorMapLength = 0;
    tgaHeader->colorMapEntrySize = 0;
    tgaHeader->xOrigin = 0;
    tgaHeader->yOrigin = 0;
    tgaHeader->width = width;
    tgaHeader->height = height;
    tgaHeader->pixelDepth = 32;
    // bit 0-3 = 0x08 (0000 1000): 1 byte alpha channel
    //                (     XXXX)
    // bit 4-5 = 0x00 (0000 0000): do not reverse x and y
    //                (  XX     )
    tgaHeader->imageDescriptor = (0x00 | 0x08);
    
    fwrite(tgaHeader, sizeof(TGAHeader), 1, stream);
    free(tgaHeader);
}

TGAError TGA_ValidateHeader(TGAHeader* tgaHeader, uint16_t width, uint16_t height) {
    if (tgaHeader->idLength != 0)
        return TGA_ERR_IDLENNEQ0;
    else if (tgaHeader->colorMapType != 0)
        return TGA_ERR_CLRMAPTYPNEQ0;
    else if (tgaHeader->imageType != 2)
        return TGA_ERR_IMGTYPNEQ2;
    else if (tgaHeader->xOrigin != 0)
        return TGA_ERR_XORGNEQ0;
    else if (tgaHeader->yOrigin != 0)
        return TGA_ERR_YORGNEQ0;
    else if (tgaHeader->width != width)
        return TGA_ERR_INVLDWIDTH;
    else if (tgaHeader->height != height)
        return TGA_ERR_INVLDHEIGHT;
    else if (tgaHeader->pixelDepth != 32)
        return TGA_ERR_PIXDEPNEQ32;
    else if (tgaHeader->imageDescriptor != (0x00 | 0x08))
        return TGA_ERR_INVLDIMGDESC;
    else
        return TGA_ERR_SUCCESS;
}

char* TGA_GetErrorStr(TGAError tgaError) {
    switch (tgaError) {
        case TGA_ERR_SUCCESS:
            return "TGA_ERR_SUCCESS: Operation was successful.";
        case TGA_ERR_IDLENNEQ0:
            return "TGA_ERR_IDLENNEQ0: ID Length not equal to 0.";
        case TGA_ERR_CLRMAPTYPNEQ0:
            return "TGA_ERR_CLRMAPTYPNEQ0: Color Map Type not equal to 0.";
        case TGA_ERR_IMGTYPNEQ2:
            return "TGA_ERR_IMGTYPNEQ2: Image Type not equal to 2.";
        case TGA_ERR_XORGNEQ0:
            return "TGA_ERR_XORGNEQ0: X Origin not equal to 0.";
        case TGA_ERR_YORGNEQ0:
            return "TGA_ERR_YORGNEQ0: Y Origin not equal to 0.";
        case TGA_ERR_INVLDWIDTH:
            return "TGA_ERR_INVLDWIDTH: Invalid width.";
        case TGA_ERR_INVLDHEIGHT:
            return "TGA_ERR_INVLDHEIGHT: Invalid height.";
        case TGA_ERR_PIXDEPNEQ32:
            return "TGA_ERR_PIXDEPNEQ32: Pixel Depth not equal to 32.";
        case TGA_ERR_INVLDIMGDESC:
            return "TGA_ERR_INVLDIMGDESC: Invalid image descriptor.";
        default:
            return "Invalid error code.";
    }
}
