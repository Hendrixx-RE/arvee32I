#include <cstdint>
#include <vector>

class CPU {
public:
  std::vector<uint32_t> Registers;
  uint32_t programCounter;
  uint32_t Instruction;
  CPU();
  // Operations
  void ADD();
  void SUB();
};
