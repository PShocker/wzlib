#include "Key.h"
#include "AES.h"
#include <cstdint>

void wz::Key::ensure_key_size(size_t size) {
  
  static std::array<uint8_t, 4> iv{0, 0, 0, 0};
  static uint8_t aes_key[] = {0x13, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
                              0x06, 0x00, 0x00, 0x00, 0xB4, 0x00, 0x00, 0x00,
                              0x1B, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00,
                              0x33, 0x00, 0x00, 0x00, 0x52, 0x00, 0x00, 0x00};

  const auto batch_size = 16;
  size = ((size + batch_size - 1) / batch_size) * batch_size;

  if (*reinterpret_cast<int32_t *>(iv.data()) == 0) {
    decltype(keys) new_keys;
    new_keys.reserve(size);
    keys = new_keys;
    return;
  }

  if (size <= keys.size())
    return;

  size_t old_size = keys.size();
  keys.resize(size); // 直接扩容，避免临时容器
  AES aes(256, 128);

  // 生成第一块（如果需要）
  if (old_size == 0) {
    uint8_t block[16];
    for (int n = 0; n < 16; ++n)
      block[n] = iv[n % 4];

    uint32_t out_len;
    auto *eb = aes.EncryptECB(block, 16, aes_key, out_len);
    std::memcpy(keys.data(), eb, 16);
    delete[] eb;
    old_size = 16;
  }

  // 生成剩余块
  for (size_t i = old_size; i < size; i += 16) {
    uint32_t out_len;
    auto *eb = aes.EncryptECB(keys.data() + i - 16, 16, aes_key, out_len);
    std::memcpy(keys.data() + i, eb, 16);
    delete[] eb;
  }
}

uint8_t &wz::Key::operator[](size_t index) {
  if (keys.empty() || keys.size() <= index) {
    ensure_key_size(index);
  }
  return keys[index];
}