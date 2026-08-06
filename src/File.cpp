#include "File.h"
#include "Directory.h"
#include "Reader.h"
#include <cassert>
#include <charconv>
#include <cstdint>
#include <string>

wz::File::File(const char *path)
    : reader(Reader(path)), root(new Node(Type::NotSet, this)) {
  parse();
}

wz::File::~File() { delete root; }

wz::Node *wz::File::get_root() const { return root; }

bool wz::File::parse_directories(wz::Node *node) {
  auto entry_count = reader.read_compressed_int();

  for (int i = 0; i < entry_count; ++i) {
    auto type = reader.read_u8();
    size_t prevPos = 0;
    std::u16string name;

    if (type == 1) {
      reader.skip(sizeof(int32_t) + sizeof(uint16_t));
      get_wz_offset();
      continue;
    } else if (type == 2) {
      auto stringOffset = (int32_t)reader.read_u32();
      type = reader.read_wz_string_from_offset(desc.start + stringOffset, name);
    } else if (type == 3 || type == 4) {
      name = reader.read_wz_string();
    } else {
      assert(0);
    }
    auto size = reader.read_compressed_int();

    auto checksum = reader.read_compressed_int();
    auto offset = get_wz_offset();

    if (offset >= reader.size())
      return false;

    if (type == 3) {
      if (node != nullptr) {
        auto *dir = new Directory(this, false, size, offset);
        node->appendChild({name.begin(), name.end()}, dir);
      }

    } else {
      if (node != nullptr) {
        auto *dir = new Directory(this, true, size, offset);
        node->appendChild({name.begin(), name.end()}, dir);
      } else {
        prevPos = reader.get_position();
        reader.set_position(offset);
        if (!reader.is_wz_image())
          return false;
        reader.set_position(prevPos);
      }
    }
  }
  return true;
}

bool wz::File::parse() {
  auto magic = reader.read_string(4);
  if (magic != u"PKG1")
    return false;

  auto fileSize = reader.read_u64();
  auto startAt = reader.read_u32();

  auto copyright = reader.read_string();

  reader.set_position(startAt);

  auto encryptedVersion = reader.read_u16();

  auto prev_position = reader.get_position();

  for (int i = 0; i < INT16_MAX; ++i) {
    int16_t file_version = i;
    auto version_hash = wz::get_version_hash(encryptedVersion, file_version);
    if (version_hash != 0) {
      desc.start = startAt;
      desc.hash = version_hash;
      desc.version = file_version;
      if (parse_directories(nullptr)) {
        break;
      }
      reader.set_position(prev_position);
    }
  }
  reader.set_position(prev_position);
  parse_directories(root);

  return false;
}

uint32_t wz::File::get_wz_offset() {
  auto filePos = reader.get_position();
  uint32_t encryptedOffset = reader.read_u32();
  uint32_t offset = (int)(filePos - 0x3C) ^ 0xFFFFFFFF;
  int distance;

  offset *= desc.hash;
  offset -= wz::OffsetKey;
  distance = (int)offset & 0x1F;
  offset = (offset << distance) | (offset >> (32 - distance));
  offset ^= encryptedOffset;
  offset += 0x78;

  return offset;
}