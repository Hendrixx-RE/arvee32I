#include "../include/memory.hpp"
#include <fstream>
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

void Memory::write32(uint32_t addr, uint32_t value) {
  if (addr + 3 >= ram.size()) {
    throw std::out_of_range("Out of range access of write32 func");
  }
  for (uint32_t i = addr; i < addr + 4; ++i) {
    ram[i] = (value >> ((i - addr) * 8)) & 0xFF;
  }
}

void Memory::write16(uint32_t addr, uint16_t value) {
  if (addr + 1 >= ram.size()) {
    throw std::out_of_range("Out of range access of write16 func");
  }
  for (uint32_t i = addr; i < addr + 2; ++i) {
    ram[i] = (value >> ((i - addr) * 8)) & 0xFF;
  }
}

void Memory::write8(uint32_t addr, uint8_t value) {
  if (addr >= ram.size()) {
    throw std::out_of_range("Out of range access of write8 func");
  }
  ram[addr] = value;
}

void Memory::load_binary(const std::string &path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file) {
    throw std::runtime_error("file cannot be read");
  }
  int filesize = file.tellg();
  if (filesize > int(ram.size())) {
    throw std::out_of_range("too big of a binary");
  }
  file.seekg(0);
  char *poin = reinterpret_cast<char *>(ram.data());
  file.read(poin, filesize);
  file.close();
}
