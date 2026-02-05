#pragma once
#include <cstdint>
#include <string>
#include <vector>

class Memory {
public:
  Memory();
  uint32_t read32(uint32_t addr);
  uint16_t read16(uint32_t addr);
  uint8_t read8(uint32_t addr);
  void write32(uint32_t addr, uint32_t value);
  void write16(uint32_t addr, uint16_t value);
  void write8(uint32_t addr, uint8_t value);
  void load_binary(const std::string &path);

private:
  std::vector<uint8_t> ram;
};
