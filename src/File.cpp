#include "File.h"
#include "Directory.h"
#include "Reader.h"
#include <cassert>
#include <charconv>
#include <cstdint>
#include <string>

wz::File::File(const char *path)
    : reader(Reader(path)), root(new Node(Type::NotSet, this)) {}

wz::File::~File() { delete root; }

wz::Node *wz::File::get_root() const
{
    return root;
}

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
      auto *dir = new Directory(this, false, size, offset);
      node->appendChild({name.begin(), name.end()}, dir);
    } else {
      auto *dir = new Directory(this, true, size, offset);
      node->appendChild({name.begin(), name.end()}, dir);
    }
  }

  //   if (node != nullptr) {
  //     for (auto &it : *node) {
  //       for (auto child : it.second) {
  //         auto *dir = dynamic_cast<Directory *>(child);

  //         if (dir != nullptr) {
  //           if (!dir->is_image()) {
  //             reader.set_position(dir->get_offset());
  //             parse_directories(dir);
  //           }
  //         }
  //       }
  //     }
  //   }

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

  auto encryptedVersion = (int16_t)reader.read_u16();

  for (int i = 0; i < 0x7FFF; ++i) {
    int16_t file_version = static_cast<decltype(file_version)>(i);
    auto version_hash = wz::get_version_hash(encryptedVersion, file_version);

    if (version_hash != 0) {
      desc.start = startAt;
      desc.hash = version_hash;
      desc.version = file_version;

      parse_directories(root);
      return true;
    }
  }

  return false;
}

uint32_t wz::File::get_wz_offset() {
  uint32_t offset = static_cast<uint32_t>(reader.get_position());
  offset = ~(offset - desc.start);
  offset *= desc.hash;
  offset -= wz::OffsetKey;
  offset = (offset << (offset & 0x1Fu)) | (offset >> (32 - (offset & 0x1Fu)));
  uint32_t encryptedOffset = reader.read_u32();
  offset ^= encryptedOffset;
  offset += desc.start * 2;
  return offset;
}