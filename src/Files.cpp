#include "Files.h"
#include "File.h"
#include "Node.h"
#include <cassert>
#include <charconv>
#include <cstdint>
#include <format>
#include <ranges>

std::string wz::Files::parse_str_path_front(const std::string &path) {
  size_t pos = path.find_last_of('/');
  if (pos == std::string::npos) {
    return path; // 没有斜杠，返回原字符串
  }
  return path.substr(0, pos);
}

void wz::Files::parse_sub_node(const std::string &path) {

  auto front = parse_str_path_front(path);

  std::string full_path = "Data/" + path + ".wz";

  File file(full_path.c_str());
  file.parse();

  for (auto node : file.get_root()->children) {
    auto name = node.first;

    auto name2 = std::string{name.begin(), name.end()};
    full_path = front + "/" + name2 + "/" + name2;
    auto f = new Files(full_path.c_str());

    files.insert({name, f});
  }
}

uint32_t wz::Files::parse_ini_num(const std::string &path) {
  auto ini_num = 0;

  std::string full_path = "Data/" + path + ".ini";

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

  std::string full_path = "Data/" + path;

  for (uint8_t i = 0; i <= num; i++) {
    auto f_path = full_path + "_" + std::format("{:03d}", i) + ".wz";

    auto f = new File(f_path.c_str());
    f->parse();

    children.push_back(f);
  }
}

wz::Files::Files(const std::string &path) {
  parse_sub_node(path);
  parse_sub_wz(path);
}

wz::Node *wz::Files::find(const std::u16string &path) {
  wz::Node *node = nullptr;

  size_t pos = path.find(u'/');

  if (pos == std::string::npos) {
    // 没有找到分隔符，整个字符串作为第一部分
    pos = path.size();
  }

  auto first = path.substr(0, pos);
  std::u16string second = pos != path.size() ? path.substr(pos + 1) : u"";

  if (files.contains(first)) {
    node = files.at(first)->find(second);
  } else {
    for (auto nodes : children) {
      node = nodes->get_root()->find(path);
      if (node != nullptr) {
        break;
      }
    }
  }
  if (node == nullptr) {
    assert(0);
  }
  return node;
}