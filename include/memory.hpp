#pragma once
#include <cstdint>
#include <string>
#include <vector>
inline constexpr int MEM_SIZE = 128 * 1024 * 1024;

class Memory {
public:
  Memory();
  uint32_t convert(uint32_t addr);
  uint32_t read32(uint32_t addr);
  uint16_t read16(uint32_t addr);
  uint8_t read8(uint32_t addr);
  void write32(uint32_t addr, uint32_t value);
  void write16(uint32_t addr, uint16_t value);
  void write8(uint32_t addr, uint8_t value);
  void load_binary(const std::string &path);
  void load_elf(const std::string &filename);
  void setstart(uint32_t val);
  uint32_t getstart();
  uint32_t getmemval(uint32_t addr);

private:
  uint32_t start;
  std::vector<uint8_t> ram;
};
