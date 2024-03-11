#include "tool_logger.h"

tool_logger logger(Serial, "Default", true, true);

// Define the global logger object outside the class in your main code to use it globally, e.g., Logger logger(Serial, "MAIN", true);

tool_logger::tool_logger(Stream &stream, const char *identifier, bool enabled, bool id_enable) : outputStream(stream), identifier(identifier), loggingEnabled(enabled), identifierEmabled(id_enable){}

void tool_logger::enableLogging(bool enabled) {
  loggingEnabled = enabled;
}

void tool_logger::enableIdentifier(bool id_enable) {
  identifierEmabled = id_enable;
}

void tool_logger::log(const char *message) {
  if (!loggingEnabled) return;
  if (identifierEmabled){
    outputStream.print(identifier);
    outputStream.print(": ");
  }
  outputStream.print(message);
}

void tool_logger::logln(const char *message) {
  if (!loggingEnabled) return;
  if (identifierEmabled){
    outputStream.print(identifier);
    outputStream.print(": ");
  }
  outputStream.println(message);
}

void tool_logger::logf(const char *format, ...) {
  if (!loggingEnabled) return;
  char buffer[256]; // Consider dynamically allocating buffer if longer messages are needed
  va_list args;
  va_start(args, format);
  int ret = vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  if (ret < 0) {
    // Handle snprintf error. For simplicity, this is not implemented here.
  }
  if (identifierEmabled){
    outputStream.print(identifier);
    outputStream.print(": ");
  }
  outputStream.print(buffer);
}

void tool_logger::loglnf(const char *format, ...) {
  if (!loggingEnabled) return;
  char buffer[256];
  va_list args;
  va_start(args, format);
  int ret = vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  if (ret < 0) {
    // Handle snprintf error. For simplicity, this is not implemented here.
  }
  if (identifierEmabled){
    outputStream.print(identifier);
    outputStream.print(": ");
  }
  outputStream.println(buffer);
}
