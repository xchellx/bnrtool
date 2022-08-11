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

#include "bnrtool.h"

int main(int argc, char** argv) {
    bnrtoolMode mode = MODE_INVALID;
    if (argc > 1) {
        if (!strcmp(argv[1], "export"))
            mode = MODE_EXPORT;
        else if (!strcmp(argv[1], "import"))
            mode = MODE_IMPORT;
    }
    
    if (!mode || (argc < 5 && mode == MODE_IMPORT) || (argc < 6 && mode == MODE_IMPORT)) {
        printf("bnrtool by Yonder 2022\n"
               "bnrtool is a tool that can extract and create Nintendo GameCube Banner (.bnr) files.\n"
               "Usage:\n"
               "  %s export <infile> <imgfile> <cmtfile> [--force-img] [--force-cmt] [-s/--silent] [-f/--force]\n"
               "    infile - Input BNR file.\n"
               "    imgfile - Output image file (data will be of a tga image).\n"
               "    cmtfile - Output comment file (data will be of a txt document).\n"
               "    force-img (optional) - Force overwrite output image file.\n"
               "    force-cmt (optional) - Force overwrite output comment file.\n"
               "    s/silent (optional) - Silence output (be quiet; no output to stdout); excluding this message.\n"
               "    f/force (optional) - Force overwrite output image file and output comment file.\n"
               "  %s import <outfile> <imgfile> <cmtfile> <format> [-s/--silent] [-f/--force]\n"
               "    outfile - Output BNR file\n"
               "    imgfile - Input image file (data assumed to be of a tga image)\n"
               "    cmtfile - Input comment file (data assumed to be of a txt document)\n"
               "    format - Output BNR file format (either \"BNR1\" or \"BNR2\")\n"
               "    f/force (optional) - Force overwrite output BNR file.\n"
               "    s/silent (optional) - Silence output (be quiet; no output to stdout); excluding this message.\n"
               "Notes:\n"
               "  - The data for the image file will be (and assumed to be) of a Targa (.tga) image of \"Original TGA "
               "Format\", therefore the extra TGA footer present in the \"New TGA Format\" will be ignored. Also, the "
               "TGA is to be of a specific format. bnrtool will error out if any of the format is incorrect. A TGA in "
               "the format of 32bit BGRA with 1 bit alpha and no coordinate flipping bits set aswell as a width and "
               "height of 96 by 32 is what is accepted.\n"
               "  - The data for the comment file will be (and assumed to be) of a Text (.txt) document and expected "
               "to be 5-6 lines in BNR1 format and 35-36 in BNR2 format. Every 5 lines of text forms a comment block. "
               "In BNR1 format, there is a single comment block that can represent any language. In BNR2 format, "
               "there is 6 comment blocks, each representing a specific language. It goes in this order: Comment #1 - "
               "English, Comment #2 - German, Comment #3 - French, Comment #4 - Spanish, Comment #5 - Italian, and "
               "Comment #6 - Dutch. For each comment, there is 5 lines. The first line is the game title in short "
               "form (32 characters max, single line). The second line is the game maker (company) in short form (32 "
               "characters max, single line). The third line is the game title in long form (64 characters max, "
               "single line). The fourth line is the game maker (company) in long format (64 characters max, single "
               "line). Finally, the fifth line is the game comment (description) (128 characters max, 2 lines max).\n",
               argv[0], argv[0]);
        return STATUS_ERROR;
    }
    
    // Export and import flags
    uint8_t forceImg = 0, forceCmt = 0, silent = 0;
    
    if (mode == MODE_EXPORT) {
        // Flag handling
        if (argc > 5) {
            char* fptr;
            for (uint32_t a = 5; a < argc; a++) {
                if (argv[a][0] == '-') {
                    if (argv[a][1] == '-') {
                        if (!strcmp(argv[a], "--force-img"))
                            forceImg = 1;
                        else if (!strcmp(argv[a], "--force-cmt"))
                            forceCmt = 1;
                        else if (!strcmp(argv[a], "--silent"))
                            silent = 1;
                        else if (!strcmp(argv[a], "--force")) {
                            forceImg = 1;
                            forceCmt = 1;
                        }
                    } else {
                        fptr = argv[a];
                        while ((fptr = (fptr + 1))[0] != '\0') {
                            if (fptr[0] == 's')
                                silent = 1;
                            else if (fptr[0] == 'f') {
                                forceImg = 1;
                                forceCmt = 1;
                            }
                        }
                    }
                }
            }
        }
        
        if (!silent)
            printf("bnrtool by Yonder 2022\n"
                   "Tool mode: export\n");
        
        FILE* bnrin;
        if (!(bnrin = fopen(argv[2], "rb"))) {
            if (!silent)
                printf("ERROR: Couldn't open input file \"%s\"\n", argv[2]);
            return STATUS_IOERROR;
        }
        
        // BNR: Magic - 0x424E5231 (BNR1) or 0x424E5232 (BNR2)
        uint32_t magic;
        fread(&magic, sizeof(uint32_t), 1, bnrin);
        magic = ED_SWAP32(magic);
        
        uint32_t isBNR2 = (magic == BNR2_MAGIC);
        if (magic != BNR1_MAGIC && !isBNR2) {
            if (!silent)
                printf("ERROR: Input file is not a .bnr file\n");
            fclose(bnrin);
            return STATUS_INVLDARG;
        }
        
        if (!silent)
            printf("BNR format: %s\n", (isBNR2 ? "BNR2" : "BNR1"));
        
        // BNR: Read BNR
        size_t bnrSz = (isBNR2 ? sizeof(BNR2) : sizeof(BNR1));
        void* bnr = (void*) malloc(bnrSz);
        fread(bnr, bnrSz, 1, bnrin);
        
        fclose(bnrin);
        
        // BNR: Image - 96x32, RGB5A3
        if (!forceImg) {
            if (!silent)
                printf("Do you want to overwrite output image file \"%s\"? (y,Y/ANY) ", argv[3]);
            char yn = cgetchar(!silent);
            if (yn != 'y' && yn != 'Y') {
                if (!silent)
                    printf("ERROR: Not overwriting output image file \"%s\".\n", argv[3]);
                free(bnr);
                return STATUS_INVLDARG;
            }
        }
        FILE* bnrimgout;
        if (!(bnrimgout = fopen(argv[3], "wb"))) {
            if (!silent)
                printf("ERROR: Couldn't open output image file \"%s\"\n", argv[3]);
            free(bnr);
            return STATUS_IOERROR;
        }
        
        uint16_t imgWidth = 96, imgHeight = 32;
        if (!silent) {
            printf("BNR image: %ux%u\n", imgWidth, imgHeight);
            printf("Parsing image data from \"%s\" and writing it to \"%s\"...\n", argv[2], argv[3]);
        }
        
        TGA_WriteHeader(imgWidth, imgHeight, bnrimgout);
        uint16_t* inpixels;
        if (isBNR2) {
            BNR2* bnr2 = (BNR2*) bnr;
            inpixels = bnr2->image;
        } else {
            BNR1* bnr1 = (BNR1*) bnr;
            inpixels = bnr1->image;
        }
        uint32_t* outpixels = (uint32_t*) malloc((imgWidth * imgHeight) * sizeof(uint32_t));
        GX_DecodeRGB5A3(imgWidth, imgHeight, inpixels, outpixels);
        fwrite(outpixels, sizeof(uint32_t), (imgWidth * imgHeight), bnrimgout);
        
        free(outpixels);
        fclose(bnrimgout);
        
        // BNR: Comment(s)
        if (!forceCmt) {
            if (!silent)
                printf("Do you want to overwrite output comment file \"%s\"? (y,Y/ANY) ", argv[3]);
            char yn = cgetchar(!silent);
            if (yn != 'y' && yn != 'Y') {
                if (!silent)
                    printf("ERROR: Not overwriting output comment file \"%s\".\n", argv[3]);
                free(bnr);
                return STATUS_INVLDARG;
            }
        }
        FILE* bnrcommentout;
        if (!(bnrcommentout = fopen(argv[4], "w"))) {
            if (!silent)
                printf("ERROR: Couldn't open output comment file %s\n", argv[4]);
            free(bnr);
            return STATUS_IOERROR;
        }
        
        if (!silent) {
            printf("BNR Comments: %u\n", (isBNR2 ? 6 : 1));
            printf("Parsing comment data from \"%s\" and writing it to \"%s\"...\n", argv[2], argv[4]);
        }
        
        if (isBNR2) {
            BNR2* bnr2 = (BNR2*) bnr;
            fprintf(bnrcommentout,
                "%.*s\n%.*s\n%.*s\n%.*s\n%.*s\n"
                "%.*s\n%.*s\n%.*s\n%.*s\n%.*s\n"
                "%.*s\n%.*s\n%.*s\n%.*s\n%.*s\n"
                "%.*s\n%.*s\n%.*s\n%.*s\n%.*s\n"
                "%.*s\n%.*s\n%.*s\n%.*s\n%.*s\n"
                "%.*s\n%.*s\n%.*s\n%.*s\n%.*s",
                // Language #1: English
                BNR_SHORTTITLESZ, bnr2->shortTitleEnglish,
                BNR_SHORTMAKERSZ, bnr2->shortMakerEnglish,
                BNR_LONGTITLESZ, bnr2->longTitleEnglish,
                BNR_LONGMAKERSZ, bnr2->longMakerEnglish,
                BNR_COMMENTSZ, bnr2->commentEnglish,
                // Language #2: German
                BNR_SHORTTITLESZ, bnr2->shortTitleGerman,
                BNR_SHORTMAKERSZ, bnr2->shortMakerGerman,
                BNR_LONGTITLESZ, bnr2->longTitleGerman,
                BNR_LONGMAKERSZ, bnr2->longMakerGerman,
                BNR_COMMENTSZ, bnr2->commentGerman,
                // Language #3: French
                BNR_SHORTTITLESZ, bnr2->shortTitleFrench,
                BNR_SHORTMAKERSZ, bnr2->shortMakerFrench,
                BNR_LONGTITLESZ, bnr2->longTitleFrench,
                BNR_LONGMAKERSZ, bnr2->longMakerFrench,
                BNR_COMMENTSZ, bnr2->commentFrench,
                // Language #4: Spanish
                BNR_SHORTTITLESZ, bnr2->shortTitleSpanish,
                BNR_SHORTMAKERSZ, bnr2->shortMakerSpanish,
                BNR_LONGTITLESZ, bnr2->longTitleSpanish,
                BNR_LONGMAKERSZ, bnr2->longMakerSpanish,
                BNR_COMMENTSZ, bnr2->commentSpanish,
                // Language #5: Italian
                BNR_SHORTTITLESZ, bnr2->shortTitleItalian,
                BNR_SHORTMAKERSZ, bnr2->shortMakerItalian,
                BNR_LONGTITLESZ, bnr2->longTitleItalian,
                BNR_LONGMAKERSZ, bnr2->longMakerItalian,
                BNR_COMMENTSZ, bnr2->commentItalian,
                // Language #6: Dutch
                BNR_SHORTTITLESZ, bnr2->shortTitleDutch,
                BNR_SHORTMAKERSZ, bnr2->shortMakerDutch,
                BNR_LONGTITLESZ, bnr2->longTitleDutch,
                BNR_LONGMAKERSZ, bnr2->longMakerDutch,
                BNR_COMMENTSZ, bnr2->commentDutch);
        } else {
            BNR1* bnr1 = (BNR1*) bnr;
            fprintf(bnrcommentout,
                "%.*s\n%.*s\n%.*s\n%.*s\n%.*s",
                // Language #1: Any
                BNR_SHORTTITLESZ, bnr1->shortTitle,
                BNR_SHORTMAKERSZ, bnr1->shortMaker,
                BNR_LONGTITLESZ, bnr1->longTitle,
                BNR_LONGMAKERSZ, bnr1->longMaker,
                BNR_COMMENTSZ, bnr1->comment);
        }
        
        fclose(bnrcommentout);
        
        free(bnr);
        if (!silent)
            printf("Done.\n");
        return STATUS_SUCCESS;
    } else if (mode == MODE_IMPORT) {
        // Flag handling
        // Flag handling
        if (argc > 6) {
            char* fptr;
            for (uint32_t a = 6; a < argc; a++) {
                if (argv[a][0] == '-') {
                    if (argv[a][1] == '-') {
                        if (!strcmp(argv[a], "--silent"))
                            silent = 1;
                        else if (!strcmp(argv[a], "--force")) {
                            forceImg = 1;
                            forceCmt = 1;
                        }
                    } else {
                        fptr = argv[a];
                        while ((fptr = (fptr + 1))[0] != '\0') {
                            if (fptr[0] == 's')
                                silent = 1;
                            else if (fptr[0] == 'f') {
                                forceImg = 1;
                                forceCmt = 1;
                            }
                        }
                    }
                }
            }
        }
        
        if (!silent)
            printf("bnrtool by Yonder 2022\n"
                   "Tool mode: import\n");
        
        // BNR: Magic - 0x424E5231 (BNR1) or 0x424E5232 (BNR2)
        uint32_t isBNR2 = !strcmp(argv[5], "BNR2");
        if (!!strcmp(argv[5], "BNR1") && !isBNR2) {
            if (!silent)
                printf("ERROR: Invalid BNR format \"%s\". Must be either \"BNR1\" or \"BNR2\"\n", argv[5]);
            return STATUS_INVLDARG;
        }
        
        if (!silent)
            printf("BNR format: %s\n", (isBNR2 ? "BNR2" : "BNR1"));
        
        uint32_t magic = (isBNR2 ? BNR2_MAGIC : BNR1_MAGIC);
        magic = ED_SWAP32(magic);
        
        // BNR: Initialize remaining to struct
        size_t bnrSz = (isBNR2 ? sizeof(BNR2) : sizeof(BNR1));
        void* bnr = (void*) malloc(bnrSz);
        uint16_t imgWidth = 96, imgHeight = 32;
        if (isBNR2) {
            BNR2* bnr2 = (BNR2*) bnr;
            memset(bnr2->padding, 0x00, BNR_PADDINGSZ);
            memset(bnr2->image, 0x00, (imgWidth * imgHeight) * sizeof(uint16_t));
            memset(bnr2->shortTitleEnglish, '\0', BNR2_MAXCOMMENTSSZ);
        } else {
            BNR1* bnr1 = (BNR1*) bnr;
            memset(bnr1->padding, 0x00, BNR_PADDINGSZ);
            memset(bnr1->image, 0x00, (imgWidth * imgHeight) * sizeof(uint16_t));
            memset(bnr1->shortTitle, '\0', BNR1_MAXCOMMENTSSZ);
        }
        
        // BNR: Image - 96x32, RGB5A3
        FILE* bnrimgin;
        if (!(bnrimgin = fopen(argv[3], "rb"))) {
            if (!silent)
                printf("ERROR: Couldn't open input image file \"%s\"\n", argv[3]);
            free(bnr);
            return STATUS_IOERROR;
        }
        
        if (!silent) {
            printf("BNR image: %ux%u\n", imgWidth, imgHeight);
            printf("Parsing image data from \"%s\" and writing it to \"%s\"...\n", argv[3], argv[2]);
        }
        
        TGAHeader* tgaHeader = (TGAHeader*) malloc(sizeof(TGAHeader));
        fread(tgaHeader, sizeof(TGAHeader), 1, bnrimgin);
        TGAError tgaError = TGA_ValidateHeader(tgaHeader, imgWidth, imgHeight);
        if (tgaError) {
            if (!silent)
                printf("ERROR: Invalid input image data. %s\n", TGA_GetErrorStr(tgaError));
            
            free(tgaHeader);
            fclose(bnrimgin);
            free(bnr);
            return STATUS_INVLDARG;
        }
        
        uint32_t* inpixels = (uint32_t*) malloc((imgWidth * imgHeight) * sizeof(uint32_t));
        fread(inpixels, sizeof(uint32_t), (imgWidth * imgHeight), bnrimgin);
        uint16_t* outpixels;
        if (isBNR2) {
            BNR2* bnr2 = (BNR2*) bnr;
            outpixels = bnr2->image;
        } else {
            BNR1* bnr1 = (BNR1*) bnr;
            outpixels = bnr1->image;
        }
        GX_EncodeRGB5A3(imgWidth, imgHeight, inpixels, outpixels);
        
        free(inpixels);
        free(tgaHeader);
        fclose(bnrimgin);
        
        // BNR: Comment(s)
        FILE* bnrcommentin;
        if (!(bnrcommentin = fopen(argv[4], "r"))) {
            if (!silent)
                printf("ERROR: Couldn't open input comment file \"%s\"\n", argv[3]);
            free(bnr);
            return STATUS_IOERROR;
        }
        
        if (!silent) {
            printf("BNR Comments: %u\n", (isBNR2 ? 6 : 1));
            printf("Parsing comment data from \"%s\" and writing it to \"%s\"...\n", argv[4], argv[2]);
        }
        
        fseek(bnrcommentin, 0, SEEK_END);
        uint64_t cmtTxtSz = ftell(bnrcommentin);
        fseek(bnrcommentin, 0, SEEK_SET);
        
        char* cmtTxt = (char*) malloc(cmtTxtSz + 1);
        memset(cmtTxt, '\0', cmtTxtSz + 1);
        fread(cmtTxt, sizeof(char), cmtTxtSz, bnrcommentin);
        char* cmtTxtPtr = cmtTxt;
        char* cmtTxtTok;
        char* cmtPtr;
        uint32_t cmtPtrOfs = 0, cmtTxtItr = 0, cmtTxtLstItr = 0, cmtTxtRem = 0;
        
        fclose(bnrcommentin);
        
        if (isBNR2) {
            BNR2* bnr2 = (BNR2*) bnr;
            cmtPtr = bnr2->shortTitleEnglish;
        } else {
            BNR1* bnr1 = (BNR1*) bnr;
            cmtPtr = bnr1->shortTitle;
        }
        
        while ((cmtTxtTok = strsep(&cmtTxt, STR_NEWLINE)) != NULL) {
            if (isBNR2 ? (cmtTxtItr < BNR2_TXTMAXLNS) : (cmtTxtItr < BNR1_TXTMAXLNS)) {
                if (cmtTxtTok[0] != '\0') {
                    if (cmtTxtLstItr == 0)
                        strncpy((cmtPtr + (cmtPtrOfs + BNR_SHORTTITLEOFS)), cmtTxtTok, BNR_SHORTTITLESZ);
                    else if (cmtTxtLstItr == 1)
                        strncpy((cmtPtr + (cmtPtrOfs + BNR_SHORTMAKEROFS)), cmtTxtTok, BNR_SHORTMAKERSZ);
                    else if (cmtTxtLstItr == 2)
                        strncpy((cmtPtr + (cmtPtrOfs + BNR_LONGTITLEOFS)), cmtTxtTok, BNR_LONGTITLESZ);
                    else if (cmtTxtLstItr == 3)
                        strncpy((cmtPtr + (cmtPtrOfs + BNR_LONGMAKEROFS)), cmtTxtTok, BNR_LONGMAKERSZ);
                    else if (cmtTxtLstItr == 4)
                        cmtTxtRem = sprintf((cmtPtr + (cmtPtrOfs + BNR_COMMENTOFS)), "%.*s", BNR_COMMENTSZ, cmtTxtTok);
                    else if (cmtTxtLstItr == 5 && (BNR_COMMENTSZ - cmtTxtRem) > 0) {
                        strncpy((cmtPtr + (cmtPtrOfs + (BNR_COMMENTOFS + cmtTxtRem))), "\n", 1);
                        strncpy((cmtPtr + (cmtPtrOfs + (BNR_COMMENTOFS + cmtTxtRem + 1))), cmtTxtTok,
                            ((BNR_COMMENTSZ - 1) - cmtTxtRem));
                    }
                }
                
                if (isBNR2) {
                    if (cmtTxtLstItr == 5) {
                        cmtPtrOfs += BNR_COMMENTSSZ;
                        cmtTxtLstItr = 0;
                        if (cmtPtrOfs == BNR2_MAXCOMMENTSSZ) {
                            cmtTxtItr = BNR2_TXTMAXLNS;
                            break;
                        }
                    } else
                        cmtTxtLstItr++;
                } else
                    cmtTxtLstItr++;
                
                cmtTxtTok = NULL;
            } else
                break;
            
            cmtTxtItr++;
        }
        if (isBNR2 ? (cmtTxtItr == (BNR2_TXTMAXLNS - 1)) : (cmtTxtItr == (BNR1_TXTMAXLNS - 1)))
            cmtTxtItr++;
        
        if (isBNR2 ? (cmtTxtItr < BNR2_TXTMAXLNS) : (cmtTxtItr < BNR1_TXTMAXLNS)) {
            if (!silent)
                printf("ERROR: Expected line count %u was not reached by iterated line count %u\n",
                    (isBNR2 ? BNR2_TXTMAXLNS : BNR1_TXTMAXLNS), cmtTxtItr);
            free(cmtTxtPtr);
            free(bnr);
            return STATUS_INVLDARG;
        }
        
        free(cmtTxtPtr);
        
        // Write BNR
        if (!forceImg) {
            if (!silent)
                printf("Do you want to overwrite output BNR file \"%s\"? (y,Y/ANY) ", argv[3]);
            char yn = cgetchar(!silent);
            if (yn != 'y' && yn != 'Y') {
                if (!silent)
                    printf("ERROR: Not overwriting output BNR file \"%s\".\n", argv[3]);
                free(bnr);
                return STATUS_INVLDARG;
            }
        }
        FILE* bnrout;
        if (!(bnrout = fopen(argv[2], "wb"))) {
            if (!silent)
                printf("ERROR: Couldn't open output file \"%s\"\n", argv[2]);
            free(bnr);
            return STATUS_IOERROR;
        }
        
        fwrite(&magic, sizeof(uint32_t), 1, bnrout);
        fwrite(bnr, bnrSz, 1, bnrout);
        
        fclose(bnrout);
        free(bnr);
        if (!silent)
            printf("Done.\n");
        return STATUS_SUCCESS;
    }
}
