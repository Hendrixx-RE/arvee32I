#include "../include/memory.hpp"
#include <elf.h>
#include <fstream>
#include <iostream>
#include <stdexcept>

Memory::Memory() : start(0), ram(MEM_SIZE) {}

uint32_t Memory::convert(uint32_t addr) { return addr - start; }

uint32_t Memory::read32(uint32_t addr) {
  uint32_t idx = convert(addr);
  if (idx + 3 >= ram.size()) {
    throw std::out_of_range("Out of range access of read32 func");
  }
  uint32_t temp = 0;
  for (int i = 0; i < 4; ++i) {
    temp |= ram[idx + i] << (8LL * i);
  }
  return temp;
}

uint16_t Memory::read16(uint32_t addr) {
  uint32_t idx = convert(addr);
  if (idx + 1 >= ram.size()) {
    throw std::out_of_range("Out of range access of read16 func");
  }
  uint16_t temp = 0;
  for (int i = 0; i < 2; ++i) {
    temp |= ram[idx + i] << (8LL * i);
  }
  return temp;
}

uint8_t Memory::read8(uint32_t addr) {
  uint32_t idx = convert(addr);
  if (idx >= ram.size()) {
    throw std::out_of_range("Out of range access of read8 func");
  }
  return ram[idx];
}

void Memory::write32(uint32_t addr, uint32_t value) {
  uint32_t idx = convert(addr);
  if (idx + 3 >= ram.size()) {
    throw std::out_of_range("Out of range access of write32 func");
  }
  for (uint32_t i = idx; i < idx + 4; ++i) {
    ram[i] = (value >> ((i - idx) * 8)) & 0xFF;
  }
}

void Memory::write16(uint32_t addr, uint16_t value) {
  uint32_t idx = convert(addr);
  if (idx + 1 >= ram.size()) {
    throw std::out_of_range("Out of range access of write16 func");
  }
  for (uint32_t i = idx; i < idx + 2; ++i) {
    ram[i] = (value >> ((i - idx) * 8)) & 0xFF;
  }
}

void Memory::write8(uint32_t addr, uint8_t value) {
  uint32_t idx = convert(addr);
  if (idx >= ram.size()) {
    throw std::out_of_range("Out of range access of write8 func");
  }
  ram[idx] = value;
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

void Memory::setstart(uint32_t val) { start = val; }
uint32_t Memory::getstart() { return start; }

void Memory::load_elf(const std::string &filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file)
    return;
  Elf32_Ehdr header;
  file.read(reinterpret_cast<char *>(&header), sizeof(header));
  if (header.e_ident[0] != 0x7F || header.e_ident[1] != 'E') {
    std::cerr << "Not a valid ELF file!" << std::endl;
    return;
  }
  setstart(header.e_entry);
  file.seekg(header.e_phoff);
  for (int i = 0; i < header.e_phnum; i++) {
    Elf32_Phdr ph;
    file.read(reinterpret_cast<char *>(&ph), sizeof(ph));
    if (ph.p_type == 1) {
      if (convert(ph.p_vaddr) + ph.p_memsz > ram.size()) {
        std::cerr << "Segment explicitly out of memory bounds!" << std::endl;
      }
      auto current_pos = file.tellg();
      file.seekg(ph.p_offset);
      file.read(reinterpret_cast<char *>(&ram[convert(ph.p_vaddr)]),
                ph.p_filesz);
      if (ph.p_memsz > ph.p_filesz) {
        for (uint32_t j = ph.p_filesz; j < ph.p_memsz; j++) {
          ram[convert(ph.p_vaddr) + j] = 0;
        }
      }
      file.seekg(current_pos);
    }
  }
}

uint32_t Memory::getmemval(uint32_t addr) { return ram[addr]; }
