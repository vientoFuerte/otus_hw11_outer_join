#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "debug_settings.h"

namespace table {

  extern std::map<int, std::string> tableA;
  extern std::map<int, std::string> tableB;

  std::string receive(const char* data, std::size_t size);
}
