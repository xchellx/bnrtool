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

#ifndef _TGA_H_
#define _TGA_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "common.h"

typedef enum __TGAError {
    TGA_ERR_SUCCESS,
    TGA_ERR_IDLENNEQ0,
    TGA_ERR_CLRMAPTYPNEQ0,
    TGA_ERR_IMGTYPNEQ2,
    TGA_ERR_XORGNEQ0,
    TGA_ERR_YORGNEQ0,
    TGA_ERR_INVLDWIDTH,
    TGA_ERR_INVLDHEIGHT,
    TGA_ERR_PIXDEPNEQ32,
    TGA_ERR_INVLDIMGDESC
} TGAError;

typedef struct __TGAHeader {
    // TGA Header
    // From: https://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf
    // From: http://www.paulbourke.net/dataformats/tga/
    // From: https://docs.fileformat.com/image/tga/
    // TGA: ID Length - Field 1 (1 byte)
    uint8_t idLength;
    // TGA: Color Map Type - Field 2 (1 byte)
    uint8_t colorMapType;
    // TGA: Image Type - Field 3 (1 byte)
    uint8_t imageType;
    // TGA: Color Map Specification - Field 4 (5 bytes)
    // TGA: First Entry Index - Field 4.1 (2 bytes)
    uint16_t firstEntryIndex;
    // TGA: Color Map Length - Field 4.2 (2 bytes)
    uint16_t colorMapLength;
    // TGA: Color Map Entry Size - Field 4.3 (1 byte)
    uint8_t colorMapEntrySize;
    // TGA: Image Specification - Field 5 (10 bytes)
    // TGA: X-origin of Image - Field 5.1 (2 bytes)
    uint16_t xOrigin;
    // TGA: Y-origin of Image - Field 5.2 (2 bytes)
    uint16_t yOrigin;
    // TGA: Image Width - Field 5.3 (2 bytes)
    uint16_t width;
    // TGA: Image Height - Field 5.4 (2 bytes)
    uint16_t height;
    // TGA: Pixel Depth - Field 5.5 (1 byte)
    uint8_t pixelDepth;
    // TGA: Image Descriptor - Field 5.6 (1 byte)
    uint8_t imageDescriptor;
    // TGA: Image ID - Field 6 (variable)
    // Not Applicable
    // TGA: Color Map Data - Field 7 (variable)
    // Not Applicable
    // TGA: Image Data - Field 8 (variable)
    // Not Applicable
} __attribute__((packed, aligned(1))) TGAHeader;

void TGA_WriteHeader(uint16_t width, uint16_t height, FILE* stream);

TGAError TGA_ValidateHeader(TGAHeader* tgaHeader, uint16_t width, uint16_t height);

char* TGA_GetErrorStr(TGAError tgaError);
#endif
