RV32I Emulator

A lightweight, functional emulator for the RISC-V RV32I (Base Integer Instruction Set).

The emulator processes instructions through a standard pipeline.


Load a binary file into the emulator:

```./rv32i_emu path/to/your/program.bin```

Supported Instructions
Type	Instructions
R-Type	add, sub, sll, slt, sltu, xor, srl, sra, or, and
I-Type	addi, slti, sltiu, xori, ori, andi, lb, lh, lw, lbu, lhu
S-Type	sb, sh, sw
B-Type	beq, bne, blt, bge, bltu, bgeu
U-Type	lui, auipc
J-Type	jal, jalr

📜 License

This project is licensed under the MIT License.

Would you like me to help you write a "Technical Challenges" section detailing how you handled things like sign-extension or branch offsets?
