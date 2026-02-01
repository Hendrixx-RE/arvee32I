#include "../include/memory.hpp"
#include <stdexcept>

Memory::Memory() : ram(64 * 1024 * 1024) {}

uint32_t Memory::read32(uint32_t addr) {
  if (addr + 3 >= ram.size()) {
    throw std::out_of_range("Out of range access of read32 func");
  }
  uint32_t temp = 0;
  for (int i = 0; i < 4; ++i) {
    temp |= ram[addr + i] << (8LL * i);
  }
  return temp;
}

uint16_t Memory::read16(uint32_t addr) {
  if (addr + 1 >= ram.size()) {
    throw std::out_of_range("Out of range access of read16 func");
  }
  uint16_t temp = 0;
  for (int i = 0; i < 2; ++i) {
    temp |= ram[addr + i] << (8LL * i);
  }
  return temp;
}

uint8_t Memory::read8(uint32_t addr) {
  if (addr >= ram.size()) {
    throw std::out_of_range("Out of range access of read8 func");
  }
  return ram[addr];
}
