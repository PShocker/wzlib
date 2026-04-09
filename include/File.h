#pragma once

#include "Node.h"
#include <cstdint>
#include <string>
#include <vector>

namespace wz {

static const uint32_t OffsetKey = 0x581C3F6D;

static const uint32_t HeaderMagic = 0x31474B50;

class File final {
public:
  File(const std::string &new_path);

  bool parse();

  Node *get_root() const;

  Node &operator[](const std::u16string &name);

private:
  std::vector<Node *> roots;

  Description desc{};

  std::string path;

  std::flat_map<std::u16string, File> children;

  bool parse_wz();

  bool parse_sub_node();

  bool parse_sub_wz();

  bool parse_directories(Node *node);

  uint32_t get_wz_offset();

  friend class Node;
};
} // namespace wz