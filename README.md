# RISC-V 32I Emulator

A high-performance RV32I (Base Integer Instruction Set) emulator written in C++ (for educational purposes). This project is designed to simulate a 32-bit RISC-V CPU, providing a sandboxed environment for running bare-metal RISC-V binaries and passing official architectural tests.

## Overview
The emulator implements the fetch-decode-execute cycle for 32-bit instructions. It features a byte-addressable memory system within a 32-bit address space, supporting Little-Endian data storage.

### Core Specifications:
- **ISA**: RV32I (Base Integer)
- **Registers**: 32 general-purpose integer registers (`x0` hardwired to 0)
- **Memory**: 32-bit addressable, byte-level access
- **ABI**: Supports both custom TUI-style syscalls and standard Linux-style `ecall` interfaces.
- **Load**: Supports both binary and elf loading

## Architecture
The emulator is structured into three primary components:
1. **CPU**: Handles the register file, program counter (PC), and the instruction pipeline.
2. **Memory**: A virtualized 4GB address space (sparse or flat implementation) with 8-bit granularity.
3. **Bus/System**: Manages the communication between the CPU and peripherals (like the System call handler).


##  Compliance & Testing
This emulator is designed to pass the **RISC-V Architectural Tests**. 

- [x] **RV32I Base**: Passes all standard computational tests (ADD, SUB, SLT, etc.).
- [x] **Control Flow**: Passes branch and jump (JAL, JALR) compliance.
- [X] **CSRs**: (Planned) Control and Status Register support.
- [ ] **M-Extension**: (Planned) Integer Multiplications and Division.


##  Building the Project

### Prerequisites
- C++17 or higher
- `riscv32-unknown-elf-` toolchain

### Compilation
```bash
cmake -B build
cmake --build build
cd build
./rv32i {loadtype} {filepath}
