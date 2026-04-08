#pragma once
namespace wz {
enum class Type {
  NotSet = 0x00,
  Directory = 0x10,
  Image = 0x20,
  Property = 0x30,

  Null = 0x31,
  Int = 0x32,
  UnsignedShort = 0x33,
  Float = 0x34,
  Double = 0x35,
  String = 0x36,

  SubProperty = 0x37,
  Canvas = 0x38,
  Vector2D = 0x39,
  Convex2D = 0x3A,
  Sound = 0x3B,
  UOL = 0x3C,
};

}