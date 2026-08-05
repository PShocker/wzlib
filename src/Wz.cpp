#include "Wz.h"
#include "Property.h"
#include <cstdint>
#include <string>
#include <vector>

uint32_t wz::get_version_hash(int32_t encryptedVersion, int32_t realVersion) {
  int sum = 0;
  std::string versionStr = std::to_string(realVersion);

  for (char ch : versionStr) {
    sum <<= 5;
    sum += static_cast<int>(ch) + 1;
  }

  int enc = 0xff ^ ((sum >> 24) & 0xFF) ^ ((sum >> 16) & 0xFF) ^
            ((sum >> 8) & 0xFF) ^ ((sum) & 0xFF);

  if (encryptedVersion == enc) {
    return static_cast<uint32_t>(sum);
  }
  return 0;
}
