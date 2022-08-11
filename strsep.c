// https://unixpapa.com/incnote/string.html
// ^ All of the C code on this page is public domain and may be used without concern for licenses.

#ifndef HAVE_STRSEP
#include "strsep.h"

char * strsep(char **sp, char *sep)
{
    char *p, *s;
    if (sp == NULL || *sp == NULL || **sp == '\0') return(NULL);
    s = *sp;
    p = s + strcspn(s, sep);
    if (*p != '\0') *p++ = '\0';
    *sp = p;
    return(s);
}
#endif
