#ifndef color_h
#define color_h

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define ANSI_RED "\033[1;31m"
#define ANSI_WHITE "\033[0;37m"
#define ANSI_YELLOW "\033[1;33m"
#define ANSI_RESET "\033[0m"

void fprintfc(FILE * flux, char color[], char * format, ...);

#endif
