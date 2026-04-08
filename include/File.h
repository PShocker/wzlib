#pragma once

namespace wz {
class File final {
public:
  File(const char *path);
  bool parse();
  Node *get_root() const;
  Node &operator[](const wzstring &name);
};
} // namespace wz