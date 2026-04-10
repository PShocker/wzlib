#pragma once

#include "File.h"
#include "Node.h"
#include <cstdint>
#include <flat_map>
#include <string>

namespace wz {

class Files final {
public:
  Files(const std::string &path);

  Node *find(const std::u16string &name);

private:
  std::flat_multimap<std::u16string, File *> children;

  void parse_sub_wz(const std::string &path);

  void parse_sub_node(const std::string &path);

  uint32_t parse_ini_num(const std::string &path);
};
} // namespace wz