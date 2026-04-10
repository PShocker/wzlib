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
  wz::init_key(iv, aes_key);
  wz::Files files("Map");
  // wz::Files files("Sound");
  // auto n = (*files[u"MapHelper.img"])[u"213"];
  auto n = files.find(u"MapHelper.img/weather/snow/0");
  // auto n = files[u"MapHelper.img"][u"weather"];
  // if (file.parse()) {
  //   wz::Node *node =
  //   file.get_root()->find_from_path(u"Map/Map1/101000000.img"); return 1;
  // }
  return 0;
}