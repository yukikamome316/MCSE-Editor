#include "log.hpp"

#include <stdarg.h>
#include <stdio.h>

#include <fstream>
#include <iostream>

std::fstream stream;
static bool is_log_opened = false;
void open_log_file() {
  stream.open("mss.log", std::ios::out | std::ios::app);
  if (!stream) {
    std::cerr << "Error opening log file" << std::endl;
  }
  is_log_opened = true;
}

void log_print(const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);

  // format the log message
  char buffer[1024];
  vsnprintf(buffer, sizeof(buffer), fmt, va);
  std::cout << buffer;
  if (!is_log_opened) {
    open_log_file();
  }
  // stream << buffer;
  // stream.flush();
  va_end(va);
}
