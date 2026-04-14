#pragma once

#include "File.h"
#include "Node.h"
#include <cstdint>
#include <flat_map>
#include <string>
#include <vector>

namespace wz {

class Files final {
public:
  Files(const std::string &path);

  Node *find(const std::u16string &path);

  Node *find(const std::string &path);

private:
  std::flat_map<std::u16string, Files *> files;

  std::vector<File *> children;

  std::string parse_str_path_front(const std::string &path);

  void parse_sub_wz(const std::string &path);

  void parse_sub_node(const std::string &path);

  uint32_t parse_ini_num(const std::string &path);
};
} // namespace wz