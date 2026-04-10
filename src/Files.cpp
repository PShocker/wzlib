#include "Files.h"
#include "File.h"
#include "Node.h"
#include <cassert>
#include <charconv>
#include <cstdint>
#include <format>

void wz::Files::parse_sub_node(const std::string &path) {
  std::string full_path = "Data/" + path + "/" + path + ".wz";

  std::u16string u_path{full_path.begin(), full_path.end()};

  File file(full_path.c_str());
  file.parse();

  for (auto node : file.get_root()->children) {
    auto name = node.first;

    auto name2 = std::string{name.begin(), name.end()};
    full_path = "Data/" + path + "/" + name2 + "/" + name2 + ".wz";
    auto f = new File(full_path.c_str());
    f->parse();

    children.insert({name, f});
  }
}

uint32_t wz::Files::parse_ini_num(const std::string &path) {
  auto ini_num = 0;

  std::string full_path = "Data/" + path + "/" + path + ".ini";
  Reader r(full_path.c_str());

  auto v = r.read_bytes(r.size());
  std::string text{v.begin(), v.end()};

  auto pos = text.find_last_of('|');
  auto result = text.substr(pos + 1);
  std::from_chars(result.data(), result.data() + result.size(), ini_num);

  r.close();
  return ini_num;
}

void wz::Files::parse_sub_wz(const std::string &path) {
  auto num = parse_ini_num(path);

  for (uint8_t i = 0; i <= num; i++) {
    auto full_path =
        "Data/" + path + "/" + path + "_" + std::format("{:03d}", i) + ".wz";

    auto f = new File(full_path.c_str());
    f->parse();

    children.insert({u"", f});
  }
}

wz::Files::Files(const std::string &path) {
  parse_sub_node(path);
  parse_sub_wz(path);
}

wz::Node *wz::Files::find(const std::u16string &name) {
  if (children.contains(name)) {
    auto f = children.equal_range(name).first->second;
    return f->get_root();
  } else {
    auto r = children.equal_range(u"");
    for (auto it = r.first; it != r.second; ++it) {
      File *file = it->second;
      auto n = file->get_root()->find(name);
      if (n != nullptr) {
        return n;
      }
    }
  }
  assert(0);
  return nullptr;
}