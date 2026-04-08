#include "Reader.h"
#include "SDL3/SDL_iostream.h"
#include <cstdint>
#include <string>
#include <vector>

wz::Reader::Reader(const char *file_path) {
  stream = SDL_IOFromFile(file_path, "r");
}

void wz::Reader::skip(int64_t num) { SDL_SeekIO(stream, num, SDL_IO_SEEK_CUR); }

void wz::Reader::set_position(int64_t num) {
  SDL_SeekIO(stream, num, SDL_IO_SEEK_SET);
}

void wz::Reader::close() { SDL_CloseIO(stream); }

int64_t wz::Reader::size() { return SDL_GetIOSize(stream); }

int64_t wz::Reader::get_position() { return SDL_TellIO(stream); }

uint8_t wz::Reader::read_u8() {
  uint8_t r;
  SDL_ReadU8(stream, &r);
  return r;
}

uint16_t wz::Reader::read_u16() {
  uint16_t r;
  SDL_ReadU16LE(stream, &r);
  return r;
}

uint32_t wz::Reader::read_u32() {
  uint32_t r;
  SDL_ReadU32LE(stream, &r);
  return r;
}

uint64_t wz::Reader::read_u64() {
  uint64_t r;
  SDL_ReadU64LE(stream, &r);
  return r;
}

std::vector<uint8_t> wz::Reader::read_bytes(uint64_t num) {
  std::vector<uint8_t> r;
  r.resize(num);
  SDL_ReadIO(stream, r.data(), num);
  return r;
}

std::string wz::Reader::read_string() {
  std::string result{};

  while (true) {
    auto c = static_cast<char>(read_u8());
    if (!c)
      break;
    result.push_back(c);
  }
  return result;
}

std::string wz::Reader::read_string(uint32_t num) {
  auto r = read_bytes(num);
  std::string result{r.begin(), r.end()};
  return result;
}

int32_t wz::Reader::read_compressed_int() {
  auto result = static_cast<int32_t>(read_u8());
  if (result == INT8_MIN)
    return (int32_t)read_u32();
  return result;
}

std::string wz::Reader::read_wz_string() {
  const auto len8 = (int8_t)read_u8();
  if (len8 == 0)
    return {};

  // 计算长度
  int32_t len;
  if (len8 > 0) {
    len = (len8 == 127) ? (int32_t)read_u32() : len8;
  } else if (len8 == -128) {
    len = (int32_t)read_u32();
  } else {
    len = -len8;
  }

  if (len <= 0)
    return {};

  std::string result;
  result.reserve(len);

  if (len8 > 0) {
    // Unicode 字符串解密
    uint16_t mask = 0xAAAA;

    for (int32_t i = 0; i < len; ++i) {
      const auto encrypted = read_u16();
      result.push_back(encrypted ^ mask ^ key[2 * i]);
      ++mask;
    }
  } else {
    // ASCII 字符串解密
    uint8_t mask = 0xAA;

    for (int32_t i = 0; i < len; ++i) {
      const auto encrypted = read_u8();
      result.push_back(static_cast<uint16_t>(encrypted ^ mask ^ key[i]));
      ++mask;
    }
  }

  return result;
}
