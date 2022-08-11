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

#include "cgetchar.h"

#define cgetch cgetchar(0)

#define cgetche cgetchar(1)

char cgetchar(char echoc) {
    int c = '\0';
    
#ifdef _WIN32
    while ((c = (echoc ? getche() : getch())) != EOF) {
        if (c != '\0') {
            if (echoc && c != '\r' && c != '\n')
                printf("\n");
            break;
        }
    }
#else
    struct termios oldf, newf;
    tcgetattr(STDIN_FILENO, &oldf);
    newf = oldf;

    if (!echoc && (newf.c_lflag & ECHO) == ECHO)
        newf.c_lflag &= ~ECHO;
    if ((newf.c_lflag & ECHONL) == ECHONL)
        newf.c_lflag &= ~ECHONL;
    if ((newf.c_lflag & ICANON) == ICANON)
        newf.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &newf);
    
    while ((c = getchar()) != EOF) {
        if (c != '\0') {
            if (echoc && c != '\r' && c != '\n')
                printf("\n");
            break;
        }
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldf);
#endif
    
    if (c == '\r')
        c = '\n';
    return (char) c;
}
