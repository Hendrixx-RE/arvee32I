#include "../include/cpu.hpp"
#include "../include/memory.hpp"
#include <iostream>
#include <stdexcept>

CPU::CPU(Memory *mem)
    : mem(mem), Registers(32), programCounter(0), Instruction(0),
      stopped(false) {}

/* <--------------R-Type-Instructions--------------------->*/
void CPU::ADD() {
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  if (rd) {
    Registers[rd] = Registers[rs1] + Registers[rs2];
  }
}

void CPU::SUB() {
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  if (rd) {
    Registers[rd] = Registers[rs1] - Registers[rs2];
  }
}

void CPU::XOR() {
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  if (rd) {
    Registers[rd] = Registers[rs1] ^ Registers[rs2];
  }
}

void CPU::OR() {
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  if (rd) {
    Registers[rd] = Registers[rs1] | Registers[rs2];
  }
}

void CPU::AND() {
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  if (rd) {
    Registers[rd] = Registers[rs1] & Registers[rs2];
  }
}

void CPU::SLTU() {
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  if (rd) {
    Registers[rs1] < Registers[rs2] ? Registers[rd] = 1 : Registers[rd] = 0;
  }
}

void CPU::SLT() {
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  int32_t v1 = Registers[rs1];
  int32_t v2 = Registers[rs2];
  if (rd) {
    v1 < v2 ? Registers[rd] = 1 : Registers[rd] = 0;
  }
}

void CPU::SLL() {
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  uint8_t val = Registers[rs2] & 0x1F;
  if (rd) {
    Registers[rd] = Registers[rs1] << val;
  }
}

void CPU::SRL() {
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  uint8_t val = Registers[rs2] & 0x1F;
  if (rd) {
    Registers[rd] = Registers[rs1] >> val;
  }
}

void CPU::SRA() {
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  int32_t v1 = Registers[rs1];
  uint8_t val = Registers[rs2] & 0x1F;
  if (rd) {
    Registers[rd] = v1 >> val;
  }
}

/* <--------------I-Type-Instructions--------------------->*/

void CPU::ADDI() {
  int32_t val = static_cast<int32_t>(Instruction) >> 20;
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  if (rd) {
    Registers[rd] = Registers[rs1] + val;
  }
}

void CPU::SLTI() {
  int32_t val = static_cast<int32_t>(Instruction) >> 20;
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  int32_t rsv = Registers[rs1];
  if (rd) {
    rsv < val ? Registers[rd] = 1 : Registers[rd] = 0;
  }
}

void CPU::SLTIU() {
  uint32_t val = static_cast<int32_t>(Instruction) >> 20;
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  if (rd) {
    Registers[rs1] < val ? Registers[rd] = 1 : Registers[rd] = 0;
  }
}

void CPU::XORI() {
  uint32_t val = static_cast<int32_t>(Instruction) >> 20;
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  if (rd) {
    Registers[rd] = Registers[rs1] ^ val;
  }
}

void CPU::ORI() {
  uint32_t val = static_cast<int32_t>(Instruction) >> 20;
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  if (rd) {
    Registers[rd] = Registers[rs1] | val;
  }
}

void CPU::ANDI() {
  uint32_t val = static_cast<int32_t>(Instruction) >> 20;
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  if (rd) {
    Registers[rd] = Registers[rs1] & val;
  }
}

void CPU::SLLI() {
  uint8_t val = (Instruction >> 20) & 0x1F;
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  if (rd) {
    Registers[rd] = static_cast<int32_t>(Registers[rs1]) << val;
  }
}

void CPU::SRLI() {
  uint8_t val = ((Instruction) >> 20) & 0x1F;
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  if (rd) {
    Registers[rd] = static_cast<int32_t>(Registers[rs1]) >> val;
  }
}

void CPU::SRAI() {
  uint8_t val = ((Instruction) >> 20) & 0x1F;
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  if (rd) {
    Registers[rd] = static_cast<int32_t>(Registers[rs1]) >> val;
  }
}

void CPU::JALR() {
  int32_t val = (static_cast<int32_t>(Instruction) >> 20);
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  if (rd) {
    Registers[rd] = programCounter + 4;
  }
  programCounter = val + Registers[rs1];
  programCounter &= 0xFFFFFFFE;
}

void CPU::LW() {
  int32_t val = (static_cast<int32_t>(Instruction) >> 20);
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint32_t address = val + Registers[rs1];
  if (address % 4 != 0) {
    throw std::runtime_error("LW crashed coz of memory alingment at " +
                             std::to_string(address));
  }
  if (rd) {
    Registers[rd] = mem->read32(address);
  }
}

void CPU::LH() {
  int32_t val = (static_cast<int32_t>(Instruction) >> 20);
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint32_t address = val + Registers[rs1];
  if (address % 2 != 0) {
    throw std::runtime_error("LH crashed coz of memory alingment at " +
                             std::to_string(address));
  }
  if (rd) {
    int16_t temp = mem->read16(address);
    Registers[rd] = static_cast<int32_t>(temp);
  }
}

void CPU::LHU() {
  int32_t val = (static_cast<int32_t>(Instruction) >> 20);
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint32_t address = val + Registers[rs1];
  if (address % 2 != 0) {
    throw std::runtime_error("LHU crashed coz of memory alingment at " +
                             std::to_string(address));
  }
  if (rd) {
    Registers[rd] = mem->read16(address);
  }
}

void CPU::LB() {
  int32_t val = (static_cast<int32_t>(Instruction) >> 20);
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint32_t address = val + Registers[rs1];
  if (rd) {
    int8_t temp = mem->read8(address);
    Registers[rd] = static_cast<int32_t>(temp);
  }
}

void CPU::LBU() {
  int32_t val = (static_cast<int32_t>(Instruction) >> 20);
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint32_t address = val + Registers[rs1];
  if (rd) {
    Registers[rd] = mem->read8(address);
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
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  int32_t val = ((Instruction >> 7) & 0x1F) | ((Instruction >> 20) & 0xFE0);
  val = (val << 20) >> 20;
  uint32_t address = val + Registers[rs1];
  if (address % 4 != 0) {
    throw std::runtime_error("SW faulted address invalid");
  }
  mem->write32(address, Registers[rs2]);
}

void CPU::SH() {
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  int32_t val = ((Instruction >> 7) & 0x1F) | ((Instruction >> 20) & 0xFE0);
  val = (val << 20) >> 20;
  uint32_t address = val + Registers[rs1];
  if (address % 2 != 0) {
    throw std::runtime_error("SH faulted address invalid");
  }
  uint16_t value = Registers[rs2] & 0xFFFF;
  mem->write16(address, value);
}

void CPU::SB() {
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  int32_t val = ((Instruction >> 7) & 0x1F) | ((Instruction >> 20) & 0xFE0);
  val = (val << 20) >> 20;
  uint32_t address = val + Registers[rs1];
  uint8_t value = Registers[rs2] & 0xFF;
  mem->write8(address, value);
}

void CPU::BEQ() {
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  uint8_t i12 = (Instruction >> 31) & 1;
  uint8_t i11 = (Instruction >> 7) & 1;
  uint8_t i10_5 = (Instruction >> 25) & 0x3F;
  uint8_t i4_1 = (Instruction >> 8) & 0xF;
  uint16_t val = i12 << 12 | i11 << 11 | i10_5 << 5 | i4_1 << 1;
  int32_t imm = (static_cast<int32_t>(val << 19)) >> 19;
  if (Registers[rs1] == Registers[rs2]) {
    programCounter += imm;
  }
}

void CPU::BNE() {
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  uint8_t i12 = (Instruction >> 31) & 1;
  uint8_t i11 = (Instruction >> 7) & 1;
  uint8_t i10_5 = (Instruction >> 25) & 0x3F;
  uint8_t i4_1 = (Instruction >> 8) & 0xF;
  uint16_t val = i12 << 12 | i11 << 11 | i10_5 << 5 | i4_1 << 1;
  int32_t imm = (static_cast<int32_t>(val << 19)) >> 19;
  if (Registers[rs1] != Registers[rs2]) {
    programCounter += imm;
  }
}

void CPU::BLT() {
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  uint8_t i12 = (Instruction >> 31) & 1;
  uint8_t i11 = (Instruction >> 7) & 1;
  uint8_t i10_5 = (Instruction >> 25) & 0x3F;
  uint8_t i4_1 = (Instruction >> 8) & 0xF;
  uint16_t val = i12 << 12 | i11 << 11 | i10_5 << 5 | i4_1 << 1;
  int32_t imm = (static_cast<int32_t>(val << 19)) >> 19;
  if (static_cast<int32_t>(Registers[rs1]) <
      static_cast<int32_t>(Registers[rs2])) {
    programCounter += imm;
  }
}

void CPU::BGE() {
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  uint8_t i12 = (Instruction >> 31) & 1;
  uint8_t i11 = (Instruction >> 7) & 1;
  uint8_t i10_5 = (Instruction >> 25) & 0x3F;
  uint8_t i4_1 = (Instruction >> 8) & 0xF;
  uint16_t val = i12 << 12 | i11 << 11 | i10_5 << 5 | i4_1 << 1;
  int32_t imm = (static_cast<int32_t>(val << 19)) >> 19;
  if (static_cast<int32_t>(Registers[rs1]) >=
      static_cast<int32_t>(Registers[rs2])) {
    programCounter += imm;
  }
}

void CPU::BLTU() {
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  uint8_t i12 = (Instruction >> 31) & 1;
  uint8_t i11 = (Instruction >> 7) & 1;
  uint8_t i10_5 = (Instruction >> 25) & 0x3F;
  uint8_t i4_1 = (Instruction >> 8) & 0xF;
  uint16_t val = i12 << 12 | i11 << 11 | i10_5 << 5 | i4_1 << 1;
  int32_t imm = (static_cast<int32_t>(val << 19)) >> 19;
  if (Registers[rs1] < Registers[rs2]) {
    programCounter += imm;
  }
}

void CPU::BGEU() {
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  uint8_t i12 = (Instruction >> 31) & 1;
  uint8_t i11 = (Instruction >> 7) & 1;
  uint8_t i10_5 = (Instruction >> 25) & 0x3F;
  uint8_t i4_1 = (Instruction >> 8) & 0xF;
  uint16_t val = i12 << 12 | i11 << 11 | i10_5 << 5 | i4_1 << 1;
  int32_t imm = (static_cast<int32_t>(val << 19)) >> 19;
  if (Registers[rs1] >= Registers[rs2]) {
    programCounter += imm;
  }
}
