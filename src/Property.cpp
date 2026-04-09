#include "Property.h"
#include "Types.h"
#include <cstdint>
#include <zlib.h>

// get ARGB4444 piexl,ARGB8888 piexl and others.....
template <> std::vector<uint8_t> wz::Property<wz::WzCanvas>::get_raw_data() {
  WzCanvas canvas = get();

  std::vector<uint8_t> data_stream;
  reader->set_position(canvas.offset);
  size_t end_offset = reader->get_position() + canvas.size;
  unsigned long uncompressed_len = canvas.uncompressed_size;
  uint8_t *uncompressed = new uint8_t[uncompressed_len];
  if (!canvas.is_encrypted) {
    data_stream = reader->read_bytes(canvas.size);
    uncompress(uncompressed, (unsigned long *)&uncompressed_len,
               data_stream.data(), data_stream.size());
  } else {
    auto wz_key = key;

    while (reader->get_position() < end_offset) {
      auto block_size = (int32_t)reader->read_u32();
      for (size_t i = 0; i < block_size; ++i) {
        auto n = wz_key[i];
        data_stream.push_back(static_cast<uint8_t>(reader->read_u8() ^ n));
      }
    }
    uncompress(uncompressed, (unsigned long *)&uncompressed_len,
               data_stream.data(), data_stream.size());
  }

  std::vector<uint8_t> pixel_stream(uncompressed,
                                    uncompressed + uncompressed_len);
  delete[] uncompressed;
  return pixel_stream;
}

// get Sound node raw data
template <> std::vector<uint8_t> wz::Property<wz::WzSound>::get_raw_data() {
  WzSound sound = get();
  std::vector<uint8_t> data_stream;

  reader->set_position(sound.offset);
  size_t end_offset = reader->get_position() + sound.size;
  data_stream = reader->read_bytes(sound.size);
  return data_stream;
}

// get uol By uol node
template <> wz::Node *wz::Property<wz::WzUOL>::get_uol() {
  auto path = get().uol;
  auto uol_node = parent->find_from_path(path);
  while (uol_node->type == wz::Type::UOL) {
    path = static_cast<wz::Property<wz::WzUOL> *>(uol_node)->get().uol;
    uol_node = uol_node->parent->find_from_path(path);
  }

  return uol_node;
}
