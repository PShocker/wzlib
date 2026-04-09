#pragma once

#include "Node.h"
#include <cstdint>
#include <string>

namespace wz {

static const uint32_t OffsetKey = 0x581C3F6D;

static const uint32_t HeaderMagic = 0x31474B50;

class File final {
public:
  File(const char *path);

  bool parse();

  Node *get_root() const;

  Node &operator[](const std::u16string &name);

private:
  Node *root;

  Description desc{};

  Reader reader;

  bool parse_directories(Node *node);

  uint32_t get_wz_offset();

  friend class Node;
};
} // namespace wz