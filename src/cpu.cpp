#include "../include/cpu.hpp"
#include "../include/memory.hpp"
#include <iostream>
#include <stdexcept>

CPU::CPU(Memory *mem)
    : mem(mem), Registers(32), programCounter(0), Instruction(0),
      stopped(false) {}

uint8_t CPU::decodereg(uint32_t Instruction, int n) {
  switch (n) {
  case 1:
    return (Instruction >> 15) & 0x1F;
  case 2:
    return (Instruction >> 20) & 0x1F;
  default:
    return (Instruction >> 7) & 0x1F;
  }
}

int32_t CPU::bhelper(uint32_t Instruction) {
  uint8_t i12 = (Instruction >> 31) & 1;
  uint8_t i11 = (Instruction >> 7) & 1;
  uint8_t i10_5 = (Instruction >> 25) & 0x3F;
  uint8_t i4_1 = (Instruction >> 8) & 0xF;
  uint16_t val = i12 << 12 | i11 << 11 | i10_5 << 5 | i4_1 << 1;
  return (static_cast<int32_t>(val << 19)) >> 19;
}

int32_t CPU::shelper(uint32_t Instruction) {
  uint8_t i5_0 = (Instruction >> 7) & 0x1F;
  uint8_t i31_25 = Instruction >> 25;
  int32_t val = i5_0 | i31_25 << 5;
  return (val << 20) >> 20;
}

/* <--------------R-Type-Instructions--------------------->*/
void CPU::ADD() {
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] =
        Registers[decodereg(Instruction, 1)] +
        Registers[decodereg(Instruction, 2)];
  }
}

void CPU::SUB() {
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] =
        Registers[decodereg(Instruction, 1)] -
        Registers[decodereg(Instruction, 2)];
  }
}

void CPU::XOR() {
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] =
        Registers[decodereg(Instruction, 1)] ^
        Registers[decodereg(Instruction, 2)];
  }
}

void CPU::OR() {
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] =
        Registers[decodereg(Instruction, 1)] |
        Registers[decodereg(Instruction, 2)];
  }
}

void CPU::AND() {
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] =
        Registers[decodereg(Instruction, 1)] &
        Registers[decodereg(Instruction, 2)];
  }
}

void CPU::SLTU() {
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 1)] < Registers[decodereg(Instruction, 2)]
        ? Registers[decodereg(Instruction, 3)] = 1
        : Registers[decodereg(Instruction, 3)] = 0;
  }
}

void CPU::SLT() {
  int32_t v1 = Registers[decodereg(Instruction, 1)];
  int32_t v2 = Registers[decodereg(Instruction, 2)];
  if (decodereg(Instruction, 3)) {
    v1 < v2 ? Registers[decodereg(Instruction, 3)] = 1
            : Registers[decodereg(Instruction, 3)] = 0;
  }
}

void CPU::SLL() {
  uint8_t val = Registers[decodereg(Instruction, 2)] & 0x1F;
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] = Registers[decodereg(Instruction, 1)]
                                           << val;
  }
}

void CPU::SRL() {
  uint8_t val = Registers[decodereg(Instruction, 2)] & 0x1F;
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] =
        Registers[decodereg(Instruction, 1)] >> val;
  }
}

void CPU::SRA() {
  int32_t v1 = Registers[decodereg(Instruction, 1)];
  uint8_t val = Registers[decodereg(Instruction, 2)] & 0x1F;
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] = v1 >> val;
  }
}

/* <--------------I-Type-Instructions--------------------->*/

void CPU::ADDI() {
  int32_t val = static_cast<int32_t>(Instruction) >> 20;
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] =
        Registers[decodereg(Instruction, 1)] + val;
  }
}

void CPU::SLTI() {
  int32_t val = static_cast<int32_t>(Instruction) >> 20;
  int32_t rsv = Registers[decodereg(Instruction, 1)];
  if (decodereg(Instruction, 3)) {
    rsv < val ? Registers[decodereg(Instruction, 3)] = 1
              : Registers[decodereg(Instruction, 3)] = 0;
  }
}

void CPU::SLTIU() {
  uint32_t val = static_cast<int32_t>(Instruction) >> 20;
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 1)] < val
        ? Registers[decodereg(Instruction, 3)] = 1
        : Registers[decodereg(Instruction, 3)] = 0;
  }
}

void CPU::XORI() {
  uint32_t val = static_cast<int32_t>(Instruction) >> 20;
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] =
        Registers[decodereg(Instruction, 1)] ^ val;
  }
}

void CPU::ORI() {
  uint32_t val = static_cast<int32_t>(Instruction) >> 20;
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] =
        Registers[decodereg(Instruction, 1)] | val;
  }
}

void CPU::ANDI() {
  uint32_t val = static_cast<int32_t>(Instruction) >> 20;
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] =
        Registers[decodereg(Instruction, 1)] & val;
  }
}

void CPU::SLLI() {
  uint8_t val = (Instruction >> 20) & 0x1F;
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] =
        static_cast<int32_t>(Registers[decodereg(Instruction, 1)]) << val;
  }
}

void CPU::SRLI() {
  uint8_t val = ((Instruction) >> 20) & 0x1F;
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] =
        static_cast<int32_t>(Registers[decodereg(Instruction, 1)]) >> val;
  }
}

void CPU::SRAI() {
  uint8_t val = ((Instruction) >> 20) & 0x1F;
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] =
        static_cast<int32_t>(Registers[decodereg(Instruction, 1)]) >> val;
  }
}

void CPU::JALR() {
  int32_t val = (static_cast<int32_t>(Instruction) >> 20);
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] = programCounter + 4;
  }
  programCounter = val + Registers[decodereg(Instruction, 1)];
  programCounter &= 0xFFFFFFFE;
}

void CPU::LW() {
  int32_t val = (static_cast<int32_t>(Instruction) >> 20);
  uint32_t address = val + Registers[decodereg(Instruction, 1)];
  if (address % 4 != 0) {
    throw std::runtime_error("LW crashed coz of memory alingment at " +
                             std::to_string(address));
  }
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] = mem->read32(address);
  }
}

void CPU::LH() {
  int32_t val = (static_cast<int32_t>(Instruction) >> 20);
  uint32_t address = val + Registers[decodereg(Instruction, 1)];
  if (address % 2 != 0) {
    throw std::runtime_error("LH crashed coz of memory alingment at " +
                             std::to_string(address));
  }
  if (decodereg(Instruction, 3)) {
    int16_t temp = mem->read16(address);
    Registers[decodereg(Instruction, 3)] = static_cast<int32_t>(temp);
  }
}

void CPU::LHU() {
  int32_t val = (static_cast<int32_t>(Instruction) >> 20);
  uint32_t address = val + Registers[decodereg(Instruction, 1)];
  if (address % 2 != 0) {
    throw std::runtime_error("LHU crashed coz of memory alingment at " +
                             std::to_string(address));
  }
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] = mem->read16(address);
  }
}

void CPU::LB() {
  int32_t val = (static_cast<int32_t>(Instruction) >> 20);
  uint32_t address = val + Registers[decodereg(Instruction, 1)];
  if (decodereg(Instruction, 3)) {
    int8_t temp = mem->read8(address);
    Registers[decodereg(Instruction, 3)] = static_cast<int32_t>(temp);
  }
}

void CPU::LBU() {
  int32_t val = (static_cast<int32_t>(Instruction) >> 20);
  uint32_t address = val + Registers[decodereg(Instruction, 1)];
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] = mem->read8(address);
  }
}

void CPU::FENCE() {}

void CPU::EBREAK() {
  std::cerr << "EBREAK initiated" << std::endl;
  stopped = true;
}

void CPU::ECALL() {
  uint32_t id = Registers[17];
  switch (id) {
  case 1:
    std::cout << Registers[10];
    break;
  case 4: {
    uint32_t startaddr = Registers[10];
    while (mem->read8(startaddr) != '\0') {
      std::cout << mem->read8(startaddr);
      ++startaddr;
    }
    break;
  }
  case 10:
    std::cerr << "program exited through ecall\n";
    stopped = true;
    break;
  case 93:
    std::cerr << "program exited through ecall with exit code " +
                     std::to_string(Registers[10]) + "\n";
    stopped = true;
    break;
  default:
    std::cerr << "unknown system call stopping now!!\n";
    stopped = true;
    break;
  }
}

void CPU::SW() {
  int32_t imm = shelper(Instruction);
  uint32_t address = imm + Registers[decodereg(Instruction, 1)];
  if (address % 4 != 0) {
    throw std::runtime_error("SW faulted address invalid");
  }
  mem->write32(address, Registers[decodereg(Instruction, 2)]);
}

void CPU::SH() {
  int32_t imm = shelper(Instruction);
  uint32_t address = imm + Registers[decodereg(Instruction, 1)];
  if (address % 2 != 0) {
    throw std::runtime_error("SH faulted address invalid");
  }
  uint16_t value = Registers[decodereg(Instruction, 2)] & 0xFFFF;
  mem->write16(address, value);
}

void CPU::SB() {
  int32_t imm = shelper(Instruction);
  uint32_t address = imm + Registers[decodereg(Instruction, 1)];
  uint8_t value = Registers[decodereg(Instruction, 2)] & 0xFF;
  mem->write8(address, value);
}

void CPU::BEQ() {
  int32_t imm = bhelper(Instruction);
  if (Registers[decodereg(Instruction, 1)] ==
      Registers[decodereg(Instruction, 2)]) {
    programCounter += imm;
  }
}

void CPU::BNE() {
  int32_t imm = bhelper(Instruction);
  if (Registers[decodereg(Instruction, 1)] !=
      Registers[decodereg(Instruction, 2)]) {
    programCounter += imm;
  }
}

void CPU::BLT() {
  int32_t imm = bhelper(Instruction);
  if (static_cast<int32_t>(Registers[decodereg(Instruction, 1)]) <
      static_cast<int32_t>(Registers[decodereg(Instruction, 2)])) {
    programCounter += imm;
  }
}

void CPU::BGE() {
  int32_t imm = bhelper(Instruction);
  if (static_cast<int32_t>(Registers[decodereg(Instruction, 1)]) >=
      static_cast<int32_t>(Registers[decodereg(Instruction, 2)])) {
    programCounter += imm;
  }
}

void CPU::BLTU() {
  int32_t imm = bhelper(Instruction);
  if (Registers[decodereg(Instruction, 1)] <
      Registers[decodereg(Instruction, 2)]) {
    programCounter += imm;
  }
}

void CPU::BGEU() {
  int32_t imm = bhelper(Instruction);
  if (Registers[decodereg(Instruction, 1)] >=
      Registers[decodereg(Instruction, 2)]) {
    programCounter += imm;
  }
}
