// tool_logger.h
#ifndef _TOOL_LOGGER_H
#define _TOOL_LOGGER_H

#include <Arduino.h>

class tool_logger {
public:
  tool_logger(Stream &stream, const char *identifier, bool enabled = true, bool id_enabled = true);

  void enableLogging(bool enabled);
  void enableIdentifier(bool id_enable);
  void log(const char *message);
  void logln(const char *message);
  void logf(const char *format, ...);
  void loglnf(const char *format, ...);

private:
  Stream &outputStream;
  const char *identifier;
  bool loggingEnabled;
  bool identifierEmabled;
};

extern tool_logger logger;

#endif // _TOOL_LOGGER_H
