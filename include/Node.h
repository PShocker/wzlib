#pragma once
#include "File.h"
#include "Types.h"
#include "Wz.h"
#include <string>

namespace wz {

class Node {
public:
  explicit Node();
  explicit Node(const Type &new_type, File *root_file);

  virtual ~Node();

  Node &operator[](const std::u16string &name);

  Node *get_child(const std::u16string &name);

  virtual void appendChild(const std::u16string &name, Node *node);
  virtual Node *get_parent() const;
};
} // namespace wz