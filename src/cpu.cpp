#include "../include/cpu.hpp"
#include "../include/memory.hpp"
#include <iostream>
#include <stdexcept>

CPU::CPU(Memory *mem)
    : stopped(false), csrvec(4096), Registers(32), Instruction(0),
      programCounter(mem->getstart()), mem(mem), changed(false) {
  Registers[2] = programCounter + MEM_SIZE - 1;
}

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
  uint32_t i12 = (Instruction >> 31) & 0x1;
  uint32_t i11 = (Instruction >> 7) & 0x1;
  uint32_t i10_5 = (Instruction >> 25) & 0x3F;
  uint32_t i4_1 = (Instruction >> 8) & 0xF;
  int32_t imm = (i12 << 12) | (i11 << 11) | (i10_5 << 5) | (i4_1 << 1);
  if (imm & 0x1000)
    imm |= 0xFFFFE000;
  return imm;
}

int32_t CPU::shelper(uint32_t Instruction) {
  uint8_t i5_0 = (Instruction >> 7) & 0x1F;
  uint8_t i31_25 = Instruction >> 25;
  int32_t val = i5_0 | i31_25 << 5;
  return (val << 20) >> 20;
}

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
        Registers[decodereg(Instruction, 1)] >> val;
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
  int32_t val = (Instruction >> 20);
  val = (val << 20) >> 20;
  uint32_t base = Registers[decodereg(Instruction, 1)];
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] = programCounter + 4;
  }
  programCounter = val + base;
  programCounter &= 0xFFFFFFFE;
  changed = true;
}

void CPU::LW() {
  int32_t val = (Instruction >> 20);
  val = (val << 20) >> 20;
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
  int32_t val = (Instruction >> 20);
  val = (val << 20) >> 20;
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
  int32_t val = (Instruction >> 20);
  val = (val << 20) >> 20;
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
  int32_t val = (Instruction >> 20);
  val = (val << 20) >> 20;
  uint32_t address = val + Registers[decodereg(Instruction, 1)];
  if (decodereg(Instruction, 3)) {
    int8_t temp = mem->read8(address);
    Registers[decodereg(Instruction, 3)] = static_cast<int32_t>(temp);
  }
}

void CPU::LBU() {
  int32_t val = (Instruction >> 20);
  val = (val << 20) >> 20;
  uint32_t address = val + Registers[decodereg(Instruction, 1)];
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] = mem->read8(address);
  }
}

void CPU::FENCE() {}

void CPU::SYSTEM() {
  uint32_t imm = (Instruction >> 20);
  if (imm == 0x1) {
    std::cerr << "EBREAK initiated at PC: " << std::hex << programCounter
              << std::endl;
    stopped = true;
  } else if (imm == 0x0) {
    uint32_t id = Registers[17];
    switch (id) {
    case 1:
      std::cout << Registers[10] << std::flush;
      break;
    case 64: {
      uint32_t startaddr = Registers[10];
      while (mem->read8(startaddr) != '\0') {
        std::cout << mem->read8(startaddr) << std::flush;
        ++startaddr;
      }
      break;
    }
    case 10:
      std::cerr << "program exited successfully\n";
      stopped = true;
      break;
    case 11:
      std::cout << static_cast<char>(Registers[10]) << std::flush;
      break;
    case 93:
      std::cerr << "program exited successfully with exit code " +
                       std::to_string(Registers[10]) + "\n";
      stopped = true;
      break;
    default:
      std::cerr << "unknown system call stopping now!!\n";
      stopped = true;
      break;
    }
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
    changed = true;
  }
}

void CPU::BNE() {
  int32_t imm = bhelper(Instruction);
  if (Registers[decodereg(Instruction, 1)] !=
      Registers[decodereg(Instruction, 2)]) {
    programCounter += imm;
    changed = true;
  }
}

void CPU::BLT() {
  int32_t imm = bhelper(Instruction);
  if (static_cast<int32_t>(Registers[decodereg(Instruction, 1)]) <
      static_cast<int32_t>(Registers[decodereg(Instruction, 2)])) {
    programCounter += imm;
    changed = true;
  }
}

void CPU::BGE() {
  int32_t imm = bhelper(Instruction);
  if (static_cast<int32_t>(Registers[decodereg(Instruction, 1)]) >=
      static_cast<int32_t>(Registers[decodereg(Instruction, 2)])) {
    programCounter += imm;
    changed = true;
  }
}

void CPU::BLTU() {
  int32_t imm = bhelper(Instruction);
  if (Registers[decodereg(Instruction, 1)] <
      Registers[decodereg(Instruction, 2)]) {
    programCounter += imm;
    changed = true;
  }
}

void CPU::BGEU() {
  int32_t imm = bhelper(Instruction);
  if (Registers[decodereg(Instruction, 1)] >=
      Registers[decodereg(Instruction, 2)]) {
    programCounter += imm;
    changed = true;
  }
}

void CPU::LUI() {
  uint32_t imm = Instruction & 0xFFFFF000;
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] = imm;
  }
}

void CPU::AUIPC() {
  uint32_t imm = Instruction & 0xFFFFF000;
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] = imm + programCounter;
  }
}

void CPU::JAL() {
  uint8_t i20 = (Instruction >> 31) & 1;
  uint8_t i19_12 = (Instruction >> 12) & 0xFF;
  uint8_t i11 = (Instruction >> 20) & 1;
  uint16_t i10_1 = (Instruction >> 21) & 0x3FF;
  int32_t imm = (i10_1 << 1) | (i11 << 11) | (i19_12 << 12) | i20 << 20;
  imm = (imm << 11) >> 11;
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] = programCounter + 4;
  }
  programCounter += imm;
  changed = true;
}

void CPU::CSRRW() {
  uint16_t loc = Instruction >> 20;
  bool is_readonly = ((loc >> 10) == 0x3);
  uint32_t temp = csrvec[loc];
  if (!is_readonly)
    csrvec[loc] = Registers[decodereg(Instruction, 1)];
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] = temp;
  }
}

void CPU::CSRRS() {
  uint16_t loc = Instruction >> 20;
  bool is_readonly = ((loc >> 10) == 0x3);
  uint32_t temp = csrvec[loc];
  if (decodereg(!is_readonly && Instruction, 1))
    csrvec[loc] = temp | Registers[decodereg(Instruction, 1)];
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] = temp;
  }
}

void CPU::CSRRC() {
  uint16_t loc = Instruction >> 20;
  bool is_readonly = ((loc >> 10) == 0x3);
  uint32_t temp = csrvec[loc];
  if (decodereg(!is_readonly && Instruction, 1))
    csrvec[loc] = temp & ~(Registers[decodereg(Instruction, 1)]);
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] = temp;
  }
}

void CPU::CSRRWI() {
  uint16_t loc = Instruction >> 20;
  bool is_readonly = ((loc >> 10) == 0x3);
  uint32_t temp = csrvec[loc];
  if (!is_readonly)
    csrvec[loc] = decodereg(Instruction, 1);
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] = temp;
  }
}

void CPU::CSRRSI() {
  uint16_t loc = Instruction >> 20;
  bool is_readonly = ((loc >> 10) == 0x3);
  uint32_t temp = csrvec[loc];
  if (!is_readonly && decodereg(Instruction, 1))
    csrvec[loc] = temp | decodereg(Instruction, 1);
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] = temp;
  }
}

void CPU::CSRRCI() {
  uint16_t loc = Instruction >> 20;
  bool is_readonly = ((loc >> 10) == 0x3);
  uint32_t temp = csrvec[loc];
  if (!is_readonly && decodereg(Instruction, 1))
    csrvec[loc] = temp & ~(decodereg(Instruction, 1));
  if (decodereg(Instruction, 3)) {
    Registers[decodereg(Instruction, 3)] = temp;
  }
}

void CPU::cycle() {
  Instruction = mem->read32(programCounter);
  uint8_t opcode = Instruction & 0x7F;
  uint8_t fun3 = (Instruction >> 12) & 0x7;
  uint8_t fun7 = ((Instruction) >> 25) & 0x7F;
  if (opcode == 0x37 || opcode == 0x17 || opcode == 0x6F) {
    fun3 = 0;
  }
  if (opcode != 0x33 && opcode != 0x13) {
    fun7 = 0;
  } else if (opcode == 0x13 && (fun3 != 0x1 && fun3 != 0x5)) {
    fun7 = 0;
  } else if (opcode == 0x73 && fun3 != 0) {
    fun7 = 0;
  }
  uint32_t index = opcode << 10 | fun3 << 7 | fun7;
  (this->*opArray[index])();
  if (!changed) {
    programCounter += 4;
  } else {
    changed = false;
  }
}
