#include "Wz.h"
#include "Property.h"
#include <cstdint>
#include <string>

uint32_t wz::get_version_hash(int32_t encryptedVersion, int32_t realVersion) {
  int32_t versionHash = 0;
  auto versionString = std::to_string(realVersion);

  auto len = versionString.size();

  for (int i = 0; i < len; ++i) {
    versionHash =
        (32 * versionHash) + static_cast<int32_t>(versionString[i]) + 1;
  }

#define HASHING(V, S) ((V >> S##u) & 0xFFu)
#define AUTO_HASH(V)                                                           \
  (0xFFu ^ HASHING(V, 24) ^ HASHING(V, 16) ^ HASHING(V, 8) ^ V & 0xFFu)

  int32_t decryptedVersionNumber =
      AUTO_HASH(static_cast<uint32_t>(versionHash));

  if (encryptedVersion == decryptedVersionNumber) {
    return static_cast<uint32_t>(versionHash);
  }

  return 0;
}
