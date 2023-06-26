#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define INFO 1
#define ERROR 2
#define DEBUG 4
#define WARNING 8

#define ANSI_RED "\033[1;31m"
#define ANSI_WHITE "\033[0;37m"
#define ANSI_YELLOW "\033[1;33m"

#define zlog(_flux_, _level_, fmt, ...) \
    print_log(_flux_, _level_, __FILENAME__, __LINE__, fmt, __VA_ARGS__);

extern int no_log;

void enable_log(int, char**);
void test_log();
void print_log(FILE*, unsigned int, char*, int, char*, ...);
char* get_start_msg_log(int, char*, int);

#endif
