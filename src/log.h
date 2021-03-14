#pragma once
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define __LOG(ll)                                                              \
  va_list args;                                                                \
  va_start(args, format);                                                      \
  _print_log(ll, format, args);                                                \
  va_end(args);

// Log levels
enum LogLevel {
  LOG_ERROR = 1 << 0,
  LOG_WARNING = 1 << 1,
  LOG_NOTICE = 1 << 2,
  LOG_INFO = 1 << 3,
  LOG_DEBUG = 1 << 4,
  LOG_ALL = ~0,
};

// sequences can be found here:
// http://man7.org/linux/man-pages/man3/strftime.3.html
#define LOG_TIMESTAMP_FORMAT "%X"
#define LOG_TIMESTAMP_BUFFER_MAX 50
#define LOG_BUFFER_MAX 16000

void set_loglevel(enum LogLevel);
char *_log_level_name(enum LogLevel);
void _print_log(enum LogLevel, const char *, va_list);

void logger(enum LogLevel, const char *, ...);
void log_error(const char *, ...);
void log_warn(const char *, ...);
void log_notice(const char *, ...);
void log_info(const char *, ...);
void log_debug(const char *, ...);

// Simple die function
void die(int, const char *, ...);
