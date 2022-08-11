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

#ifndef _BNRTOOL_H_
#define _BNRTOOL_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "common.h"
#include "strsep.h"
#include "cgetchar.h"
#include "tga.h"
#include "gxtexture.h"

typedef enum __bnrtoolMode {
    MODE_INVALID,
    MODE_IMPORT,
    MODE_EXPORT
} bnrtoolMode;

typedef enum __bnrtoolStatus {
    STATUS_SUCCESS,
    STATUS_ERROR,
    STATUS_INVLDARG,
    STATUS_IOERROR
} bnrtoolStatus;

// "BNR1"
#define BNR1_MAGIC 0x424E5231
// "BNR2"
#define BNR2_MAGIC 0x424E5232

// 32 + 32 + 64 + 64 + 128
#define BNR_COMMENTSSZ 320
// (32 + 32 + 64 + 64 + 128) * 1
#define BNR1_MAXCOMMENTSSZ 320
// (32 + 32 + 64 + 64 + 128) * 6
#define BNR2_MAXCOMMENTSSZ 1920

#define BNR_PADDINGSZ 28

#define BNR_SHORTTITLESZ 32
#define BNR_SHORTMAKERSZ 32
#define BNR_LONGTITLESZ 64
#define BNR_LONGMAKERSZ 64
#define BNR_COMMENTSZ 128

#define BNR_SHORTTITLEOFS 0
#define BNR_SHORTMAKEROFS 32
#define BNR_LONGTITLEOFS 64
#define BNR_LONGMAKEROFS 128
#define BNR_COMMENTOFS 192

#define BNR1_TXTMAXLNS 6
#define BNR2_TXTMAXLNS 36

typedef struct __BNR1 {
    // BNR1: Padding - 28 bytes
    uint8_t padding[28];
    // BNR1: Image - 96x32, RGB5A3
    uint16_t image[3072];
    // BNR1: Comment
    // 32 bytes  - Short Title
    // 32 bytes  - Short Maker
    // 64 bytes  - Long Title
    // 64 bytes  - Long Maker
    // 128 bytes - Comment
    // Language #1: Any
    char shortTitle[BNR_SHORTTITLESZ];
    char shortMaker[BNR_SHORTMAKERSZ];
    char longTitle[BNR_LONGTITLESZ];
    char longMaker[BNR_LONGMAKERSZ];
    char comment[BNR_COMMENTSZ];
} __attribute__((packed, aligned(1))) BNR1;

typedef struct __BNR2 {
    // BNR2: Padding - 28 bytes
    uint8_t padding[28];
    // BNR2: Image - 96x32, RGB5A3
    uint16_t image[3072];
    // BNR2: Comments
    // 32 bytes  - Short Title
    // 32 bytes  - Short Maker
    // 64 bytes  - Long Title
    // 64 bytes  - Long Maker
    // 128 bytes - Comment
    // Language #1: English
    char shortTitleEnglish[BNR_SHORTTITLESZ];
    char shortMakerEnglish[BNR_SHORTMAKERSZ];
    char longTitleEnglish[BNR_LONGTITLESZ];
    char longMakerEnglish[BNR_LONGMAKERSZ];
    char commentEnglish[BNR_COMMENTSZ];
    // Language #2: German
    char shortTitleGerman[BNR_SHORTTITLESZ];
    char shortMakerGerman[BNR_SHORTMAKERSZ];
    char longTitleGerman[BNR_LONGTITLESZ];
    char longMakerGerman[BNR_LONGMAKERSZ];
    char commentGerman[BNR_COMMENTSZ];
    // Language #3: French
    char shortTitleFrench[BNR_SHORTTITLESZ];
    char shortMakerFrench[BNR_SHORTMAKERSZ];
    char longTitleFrench[BNR_LONGTITLESZ];
    char longMakerFrench[BNR_LONGMAKERSZ];
    char commentFrench[BNR_COMMENTSZ];
    // Language #4: Spanish
    char shortTitleSpanish[BNR_SHORTTITLESZ];
    char shortMakerSpanish[BNR_SHORTMAKERSZ];
    char longTitleSpanish[BNR_LONGTITLESZ];
    char longMakerSpanish[BNR_LONGMAKERSZ];
    char commentSpanish[BNR_COMMENTSZ];
    // Language #5: Italian
    char shortTitleItalian[BNR_SHORTTITLESZ];
    char shortMakerItalian[BNR_SHORTMAKERSZ];
    char longTitleItalian[BNR_LONGTITLESZ];
    char longMakerItalian[BNR_LONGMAKERSZ];
    char commentItalian[BNR_COMMENTSZ];
    // Language #6: Dutch
    char shortTitleDutch[BNR_SHORTTITLESZ];
    char shortMakerDutch[BNR_SHORTMAKERSZ];
    char longTitleDutch[BNR_LONGTITLESZ];
    char longMakerDutch[BNR_LONGMAKERSZ];
    char commentDutch[BNR_COMMENTSZ];
} __attribute__((packed, aligned(1))) BNR2;

int main(int argc, char** argv);
#endif
