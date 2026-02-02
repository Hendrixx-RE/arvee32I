#pragma once
#include <cstdint>
#include <vector>

class Memory;

class CPU {
public:
  bool stopped;
  CPU(Memory *mem);
  // Operations-R
  void ADD();
  void SUB();
  void XOR();
  void OR();
  void AND();
  void SLTU();
  void SLT();
  void SLL();
  void SRL();
  void SRA();
  // Operations-I
  void ADDI();
  void SLTI();
  void SLTIU();
  void XORI();
  void ORI();
  void ANDI();
  void SLLI();
  void SRLI();
  void SRAI();
  void JALR();
  void LW();
  void LH();
  void LHU();
  void LB();
  void LBU();
  void FENCE();
  void ECALL();
  void EBREAK();
  // Operations-S
  void SW();
  void SH();
  void SB();
  // Operations-B
  void BEQ();
  void BNE();
  void BLT();
  void BGE();
  void BLTU();
  void BGEU();
  // Operations-U
  void LUI();
  void AUIPC();
  // Operations-J
  void JAL();
  // helperfuncs
  uint8_t decodereg(uint32_t Instruction, int n);
  int32_t bhelper(uint32_t Instruction);
  int32_t shelper(uint32_t Instruction);

private:
  std::vector<uint32_t> Registers;
  uint32_t programCounter;
  uint32_t Instruction;
  Memory *mem;
};
