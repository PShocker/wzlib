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
  File(const char *path);

  ~File();

  bool parse();

  Node *get_root() const;

private:
  Node *root;

  Description desc{};

  Reader reader;

  bool parse_directories(Node *node);

  uint32_t get_wz_offset();

  friend class Node;
};
} // namespace wz