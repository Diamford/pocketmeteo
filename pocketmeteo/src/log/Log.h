#pragma once

#include <Arduino.h>

namespace pm::log {
void begin();
}

#ifndef LOG_I
#define LOG_I(msg) pm::log::info((msg), __FILE__, __LINE__)
#define LOG_W(msg) pm::log::warn((msg), __FILE__, __LINE__)
#define LOG_E(msg) pm::log::error((msg), __FILE__, __LINE__)
#endif

namespace pm::log {
void info(const char *msg, const char *file, int line);
void warn(const char *msg, const char *file, int line);
void error(const char *msg, const char *file, int line);
}

