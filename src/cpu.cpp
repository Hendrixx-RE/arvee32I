#include "../include/cpu.hpp"

CPU::CPU() : Registers(32), programCounter(0), Instruction(0) {}

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
  uint32_t val = (static_cast<int32_t>(Instruction) >> 20) & 0x1F;
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  if (rd) {
    Registers[rd] = Registers[rs1] << val;
  }
}

void CPU::SRLI() {
  uint32_t val = (static_cast<int32_t>(Instruction) >> 20) & 0x1F;
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  if (rd) {
    Registers[rd] = Registers[rs1] >> val;
  }
}

void CPU::SRAI() {
  uint32_t val = (static_cast<int32_t>(Instruction) >> 20) & 0x1F;
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  int32_t rsv = Registers[rs1];
  if (rd) {
    Registers[rd] = rsv >> val;
  }
}
