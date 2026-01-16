#include "../include/cpu.hpp"

CPU::CPU() : Registers(32), programCounter(0), Instruction(0) {}

void CPU::ADD() {
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  if (rd != 0) {
    Registers[rd] = Registers[rs1] + Registers[rs2];
  }
}

void CPU::SUB() {
  uint8_t rd = (Instruction >> 7) & 0x1F;
  uint8_t rs1 = (Instruction >> 15) & 0x1F;
  uint8_t rs2 = (Instruction >> 20) & 0x1F;
  if (rd != 0) {
    Registers[rd] = Registers[rs1] - Registers[rs2];
  }
}
