#pragma once
#include <array>
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
  void SYSTEM();
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
  void cycle();

private:
  std::vector<uint32_t> Registers;
  uint32_t Instruction;
  uint32_t programCounter;
  Memory *mem;
};

using funcp = void (CPU::*)();

inline constexpr std::array<funcp, 131072> makeOparray() {
  std::array<funcp, 131072> table{};
  table.fill(&CPU::FENCE);

  auto add_entry = [&](uint8_t op, uint8_t f3, uint8_t f7, funcp func) {
    table[(op << 10) | (f3 << 7) | f7] = func;
  };

  // --- OP 0x33 (R-Type) ---
  add_entry(0x33, 0x0, 0x00, &CPU::ADD);
  add_entry(0x33, 0x0, 0x20, &CPU::SUB);
  add_entry(0x33, 0x1, 0x00, &CPU::SLL);
  add_entry(0x33, 0x2, 0x00, &CPU::SLT);
  add_entry(0x33, 0x3, 0x00, &CPU::SLTU);
  add_entry(0x33, 0x4, 0x00, &CPU::XOR);
  add_entry(0x33, 0x5, 0x00, &CPU::SRL);
  add_entry(0x33, 0x5, 0x20, &CPU::SRA);
  add_entry(0x33, 0x6, 0x00, &CPU::OR);
  add_entry(0x33, 0x7, 0x00, &CPU::AND);

  // --- OP-IMM 0x13 (I-Type) ---
  add_entry(0x13, 0x0, 0x00, &CPU::ADDI);
  add_entry(0x13, 0x2, 0x00, &CPU::SLTI);
  add_entry(0x13, 0x3, 0x00, &CPU::SLTIU);
  add_entry(0x13, 0x4, 0x00, &CPU::XORI);
  add_entry(0x13, 0x6, 0x00, &CPU::ORI);
  add_entry(0x13, 0x7, 0x00, &CPU::ANDI);
  add_entry(0x13, 0x1, 0x00, &CPU::SLLI);
  add_entry(0x13, 0x5, 0x00, &CPU::SRLI);
  add_entry(0x13, 0x5, 0x20, &CPU::SRAI);

  // --- LOAD 0x03, STORE 0x23, BRANCH 0x63, JUMPS (f7 = 0) and the SYSTEM CALLS
  // ---
  add_entry(0x03, 0x0, 0x00, &CPU::LB);
  add_entry(0x03, 0x1, 0x00, &CPU::LH);
  add_entry(0x03, 0x2, 0x00, &CPU::LW);
  add_entry(0x03, 0x4, 0x00, &CPU::LBU);
  add_entry(0x03, 0x5, 0x00, &CPU::LHU);
  add_entry(0x23, 0x0, 0x00, &CPU::SB);
  add_entry(0x23, 0x1, 0x00, &CPU::SH);
  add_entry(0x23, 0x2, 0x00, &CPU::SW);
  add_entry(0x63, 0x0, 0x00, &CPU::BEQ);
  add_entry(0x63, 0x1, 0x00, &CPU::BNE);
  add_entry(0x63, 0x4, 0x00, &CPU::BLT);
  add_entry(0x63, 0x5, 0x00, &CPU::BGE);
  add_entry(0x63, 0x6, 0x00, &CPU::BLTU);
  add_entry(0x63, 0x7, 0x00, &CPU::BGEU);
  add_entry(0x67, 0x0, 0x00, &CPU::JALR);
  add_entry(0x6F, 0x0, 0x00, &CPU::JAL);
  add_entry(0x37, 0x0, 0x00, &CPU::LUI);
  add_entry(0x17, 0x0, 0x00, &CPU::AUIPC);
  add_entry(0x73, 0x0, 0x00, &CPU::SYSTEM);

  return table;
}

inline constexpr std::array<funcp, 131072> opArray = makeOparray();
