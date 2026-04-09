#pragma once

#include "File.h"
#include "Node.h"
#include <cstdint>


namespace wz {
class Directory : public Node {
public:
  Directory(File *root_file, bool img, int new_size, unsigned int new_offset);

  uint32_t get_offset() const;

  bool is_image() const;

  bool parse_image(Node *node);

private:
  bool image;
  int size;
  unsigned int offset;
};
} // namespace wz