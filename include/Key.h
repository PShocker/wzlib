#pragma once

#include <cstdint>
#include <vector>
namespace wz {

static std::array<uint8_t, 4> iv;
static std::vector<uint8_t> aes_key;

class Key final {
public:
  explicit Key() = default;

  uint8_t &operator[](size_t index);
  void ensure_key_size(size_t size);

private:
  std::vector<uint8_t> keys;
};

static inline Key key;
void init_key(std::array<uint8_t, 4> iv, std::vector<uint8_t> aes_key);
} // namespace wz