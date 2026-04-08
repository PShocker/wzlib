#pragma once

#include <cstdint>
#include <vector>
namespace wz {
class Key final {
public:
  explicit Key() = default;

  uint8_t &operator[](size_t index);
  void ensure_key_size(size_t size);

private:
  std::vector<uint8_t> keys;
};

static inline Key key;
} // namespace wz