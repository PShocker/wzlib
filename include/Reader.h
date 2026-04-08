#pragma once
#include "SDL3/SDL_iostream.h"
#include <cstdint>
#include <string>
#include <vector>

namespace wz {
class Reader final {
public:
  explicit Reader(const char *file_path);
  void skip(int64_t num);

  void set_position(int64_t num);
  int64_t get_position();

  void close();
  int64_t size();

  uint8_t read_u8();
  uint16_t read_u16();
  uint32_t read_u32();
  uint64_t read_u64();

  std::vector<uint8_t> read_bytes(uint64_t num);

  std::string read_string();
  std::string read_string(uint32_t num);

  int32_t read_compressed_int();
  std::string read_wz_string();

private:
  SDL_IOStream *stream;
  
};
} // namespace wz