#include <cstdint>
#include <string>
#include <vector>

class Memory {
public:
  std::vector<uint8_t> ram;
  // // Read/Write methods
  // uint32_t read32(uint32_t addr);
  // void write32(uint32_t addr, uint32_t value);
  //
  // // For smaller types (needed for LB, LH, SB, SH instructions)
  // uint8_t read8(uint32_t addr);
  // uint16_t read16(uint32_t addr);
  //
  // bool load_binary(const std::string &path);
};
