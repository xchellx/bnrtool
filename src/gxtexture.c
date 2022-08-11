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

#include "gxtexture.h"

void GX_DecodeRGB5A3(uint16_t width, uint16_t height, uint16_t* buffin, uint32_t* buffout) {
    uint16_t inpixel;
    uint32_t outpixel;
    uint32_t inpixelidx = 0;
    for (uint32_t y = 0; y < height; y += 4) {
        for (uint32_t x = 0; x < width; x += 4) {
            for (uint32_t y1 = y; y1 < y + 4; y1++) {
                for (uint32_t x1 = x; x1 < x + 4; x1++) {
                    if (y1 < height || x1 < width) {
                        inpixel = ED_SWAP16(buffin[inpixelidx++]);
                        
                        // From: https://wiki.tockdom.com/wiki/Image_Formats#RGB5A3
                        if (((inpixel >> 15) & 0x1)) {
                            // RGB555
                            outpixel = (
                                  ((((inpixel >> 0 ) & 0x1F) * 8 ) << 0 ) /* B */
                                | ((((inpixel >> 5 ) & 0x1F) * 8 ) << 8 ) /* G */
                                | ((((inpixel >> 10) & 0x1F) * 8 ) << 16) /* R */
                                | ((((255          )       )     ) << 24) /* A */
                            );
                        } else {
                            // RGBA4443
                            outpixel = (
                                  ((((inpixel >> 0 ) & 0x0F) * 17) << 0 ) /* B */
                                | ((((inpixel >> 4 ) & 0x0F) * 17) << 8 ) /* G */
                                | ((((inpixel >> 8 ) & 0x0F) * 17) << 16) /* R */
                                | ((((inpixel >> 12) & 0x07) * 36) << 24) /* A */
                            );
                        }
                        
                        buffout[(((height - 1) - y1) * width) + x1] = outpixel;
                    } else
                        continue;
                }
            }
        }
    }
}

void GX_EncodeRGB5A3(uint16_t width, uint16_t height, uint32_t* buffin, uint16_t* buffout) {
    uint32_t inpixel;
    uint16_t outpixel;
    uint32_t outpixelidx = 0;
    for (uint32_t y = 0; y < height; y += 4) {
        for (uint32_t x = 0; x < width; x += 4) {
            for (uint32_t y1 = y; y1 < y + 4; y1++) {
                for (uint32_t x1 = x; x1 < x + 4; x1++) {
                    inpixel = ((y1 < height || x1 < width) ? buffin[(((height - 1) - y1) * width) + x1] : 0x00000000);
                    
                    // From: https://wiki.tockdom.com/wiki/Image_Formats#RGB5A3
                    if (((inpixel >> 24) & 0xFF) >= 224) {
                        // RGB555
                        outpixel = (0x8000 | (
                            // None                                            /* A */
                              (((((inpixel >> 16) & 0xFF) / 8 ) & 0x1F) << 10) /* R */
                            | (((((inpixel >> 8 ) & 0xFF) / 8 ) & 0x1F) << 5 ) /* G */
                            | (((((inpixel >> 0 ) & 0xFF) / 8 ) & 0x1F) << 0 ) /* B */
                        ));
                    } else {
                        // RGBA4443
                        outpixel = (0x0000 | (
                              (((((inpixel >> 24) & 0xFF) / 36) & 0x07) << 12) /* A */
                            | (((((inpixel >> 16) & 0xFF) / 17) & 0x0F) << 8 ) /* R */
                            | (((((inpixel >> 8 ) & 0xFF) / 17) & 0x0F) << 4 ) /* G */
                            | (((((inpixel >> 0 ) & 0xFF) / 17) & 0x0F) << 0 ) /* B */
                        ));
                    }
                    
                    buffout[outpixelidx++] = ED_SWAP16(outpixel);
                }
            }
        }
    }
}
