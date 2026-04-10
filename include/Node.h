#pragma once
#include "Reader.h"
#include "Types.h"
#include "Wz.h"
#include <flat_map>
#include <string>
#include <vector>

namespace wz {

class Node;
class File;

typedef std::vector<Node *> WzList;
typedef std::flat_map<std::u16string, WzList> WzMap;

class Node {
public:
  explicit Node();
  explicit Node(const Type &new_type, File *root_file);
  explicit Node(const std::u16string &new_path, File *root_file);

  Node *get_child(const std::u16string &name);

  Node *get_child(const std::string &name);

  Node *get_parent() const;

  std::u16string get_path() const;

  size_t children_count() const;

  void appendChild(const std::u16string &name, Node *node);

  WzMap::iterator begin();

  WzMap::iterator end();

  Node *find(const std::u16string &path);

  Node *find(const std::string &path);

public:
  Type type;

  Node *parent;
  WzMap children;

  File *file;
  Reader *reader = nullptr;

  std::u16string path = u"";

  bool parse_property_list(Node *target, size_t offset);
  void parse_extended_prop(const std::u16string &name, Node *target,
                           const size_t &offset);
  WzCanvas parse_canvas_property();
  WzSound parse_sound_property();

  friend class Directory;
};
} // namespace wz