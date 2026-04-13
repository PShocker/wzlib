#pragma once

#include "Key.h"
#include "Node.h"
#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <utility>


namespace wz {
template <typename T> class Property : public Node {
public:
  explicit Property(const Type &new_type, File *root_file)
      : Node(new_type, root_file) {}

  explicit Property(const Type &new_type, File *root_file, T new_data)
      : data(new_data), Node(new_type, root_file) {}

  void set(T new_data) { data = new_data; }

  [[maybe_unused]] const T &get() const { return data; }

  [[nodiscard]] [[maybe_unused]] std::vector<uint8_t> get_raw_data();

  [[nodiscard]] [[maybe_unused]] wz::Node *get_uol();

private:
  T data;
};
} // namespace wz
