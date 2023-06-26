#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "log.h"

int no_log = 1;

void enable_log(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stdout, "To enable logs use : -l argument\n");
    }
    else {
	if (argv[1][0] == '-' && argv[1][1] == 'l') {
	    no_log = 0;
	    zlog(stdout, INFO, "logs enable", NULL);
	}
	else fprintf(stdout, "non-valid argument\n");
    }
}

void test_log(void)
{
    int random_log;
    int n = 15;

    srand(time(NULL));
    while (n--) {
	random_log = rand()%4+1;
	switch(random_log) {
	case 1:
	    zlog(stdout, INFO, "salut", "oui");
	    zlog(stdout, INFO, "%s", "Informations utiles");
	    break;
	case 2:
	    zlog(stdout, WARNING,
		 "Attention : '%s'", "Faites attention");
	    break;
	case 3:
	    zlog(stdout, ERROR, "Fatal Error : '%s'", "Test");
	    break;
	case 4:
	    zlog(stdout, DEBUG, "n=%d", n);
	    break;
	}
	sleep(1);
    }
}

char* get_start_msg_log(int log_level,
			char* file_name,
			int line_number)
{
    /* char buffer[2048]; */
    char* buffer = (char*) malloc(sizeof(char)*200);
    char title[8] = "UNKNOWN";
    char color[11] = ANSI_WHITE;
    
    switch(log_level) {
    case INFO:
	strncpy(title, " INFO  ", 8);
	strncpy(color, ANSI_WHITE, 11);
	break;
    case ERROR:
	strncpy(title, " ERROR ", 8);
	strncpy(color, ANSI_RED, 11);
	break;
    case DEBUG:
	strncpy(title, " DEBUG ", 8);
	strncpy(color, ANSI_WHITE, 11);
	break;
    case WARNING:
	strncpy(title, "WARNING", 8);
	strncpy(color, ANSI_YELLOW, 11);
	break;
    }
    sprintf(buffer, "%-20s:%5d   %s[%s]   \033[0m",
	    file_name,
	    line_number,
	    color,
	    title);
    return buffer;
}

void print_log(FILE* flux,
	       unsigned int log_level,
	       char* file_name,
	       int line_number,
	       char* format,
	        ...)
{
    char buffer[2048];
    va_list args;
    char* buffer_start;

    if (flux == NULL) {flux = stdout;}
    if (!no_log) {
	if (log_level == INFO
	    || log_level == ERROR
	    || log_level == DEBUG
	    || log_level == WARNING) {

	    va_start(args, format);
	    vsprintf(buffer, format, args);
	    buffer_start = get_start_msg_log(log_level,
					     file_name,
					     line_number);
	    fprintf(flux, "%s%s\n", buffer_start, buffer);
	    fflush(flux);
	    free(buffer_start);
	}
    }
}
