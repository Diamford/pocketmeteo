#include "Log.h"

namespace pm::log {

void begin() {}

static void logLine(const char *level, const char *msg, const char *file,
                    int line) {
  Serial.print('[');
  Serial.print(level);
  Serial.print("] ");
  Serial.print(msg);
  Serial.print(" (");
  Serial.print(file);
  Serial.print(':');
  Serial.print(line);
  Serial.println(')');
}

void info(const char *msg, const char *file, int line) {
  logLine("I", msg, file, line);
}

void warn(const char *msg, const char *file, int line) {
  logLine("W", msg, file, line);
}

void error(const char *msg, const char *file, int line) {
  logLine("E", msg, file, line);
}

} // namespace pm::log

