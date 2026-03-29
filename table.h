#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "debug_settings.h"
#include <map>

namespace table {

  std::map<int, std::string> tableA;
  std::map<int, std::string> tableB;


  void receive(const char* data, std::size_t size);
}
