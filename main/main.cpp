#include "Key.h"
#include <iostream>

#include <algorithm>

#include <Directory.h>
#include <Files.h>
#include <Node.h>
#include <Property.h>

#include <fstream>
#include <iostream>

static std::array<uint8_t, 4> iv{0x4d, 0x23, 0xc7, 0x2b};
static std::vector<uint8_t> aes_key = {
    0x13, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
    0x00, 0xB4, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x0F, 0x00,
    0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x52, 0x00, 0x00, 0x00};

int main() {
  // wz::init_key(iv, aes_key);
  wz::Files files("Data/", "Character/Character");
  auto n = files.find(u"00002000.img");
  // auto info=n->find(u"info");
  // auto t = static_cast<wz::Property<wz::WzCanvas> *>(n)->get();
  // wz::File file("Data/Character/Character_000.wz");
  // wz::File file("Data/sdlms.wz");
  // auto b = file.get_root()->find("String.img/Job/000");
  // auto bs = static_cast<wz::Property<std::u16string> *>(b)->get();
  return 0;
}