#include "log.hpp"

#include <stdarg.h>
#include <stdio.h>

#include <fstream>
#include <iostream>

std::fstream stream;

void open_log_file(int mode, int reason) {
  stream.open("mss.log", std::ios::out | std::ios::app);
  if (!stream) {
    std::cerr << "Error opening log file" << std::endl;
  }
}

void log_print(const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);

  // format the log message
  char buffer[1024];
  vsnprintf(buffer, sizeof(buffer), fmt, va);
  std::cout << buffer;
  // stream << buffer;
  // stream.flush();
  va_end(va);
}
