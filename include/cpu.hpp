#include <cstdint>
#include <vector>

class CPU {
public:
  std::vector<uint32_t> Registers;
  uint32_t programCounter;
  uint32_t Instruction;
  CPU();
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
};
