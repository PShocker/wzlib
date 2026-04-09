#include "Directory.h"
#include <cstdint>

wz::Directory::Directory(File *root_file, bool img, int new_size,
                         unsigned int new_offset)
    : image(img), size(new_size), offset(new_offset),
      Node(img ? Type::Image : Type::Directory, root_file) {}

bool wz::Directory::parse_image(Node *node) {
  if (is_image()) {
    const auto current_offset = get_offset();
    reader->set_position(current_offset);
    if (reader->is_wz_image()) {
      return parse_property_list(node, current_offset);
    }
  }
  return false;
}

uint32_t wz::Directory::get_offset() const { return offset; }

bool wz::Directory::is_image() const { return image; }
