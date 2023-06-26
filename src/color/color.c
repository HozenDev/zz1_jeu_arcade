#include "color.h"

void fprintfc(FILE * flux, char color[], char * format, ...)
{
    char buffer[2048];
    va_list args;
    if (flux == NULL) {flux = stdout;}
    va_start(args, format);
    vsprintf(buffer, format, args);
    fprintf(flux, "%s%s%s", color, buffer, ANSI_RESET);
}
