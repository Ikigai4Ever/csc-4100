# Chapter 1 Notes

## Introduction

RISC pattern is Load/Store

- Load data from RAM to registers
- Manipulate data in registers
- Store data from registers to RAM

On AARCH64 (also known as ARMv8) is RISC

- Math and logic instructions can only access registers

## Registers in the AARCH64

<img width="672" height="433" alt="image" src="https://github.com/user-attachments/assets/c6f0b5d0-affd-40d6-bf75-3cfb25de0311" />

### General-Purpose Registers

Registers X0 through X30 are 64-bit registers

These are used to hold data for instructions, used to pass parameters to and from functions, and are used to hold the return address for function calls (X0).

Some have special purposes defined in hardware (e.g. X30) or defined by software convention (e.g. X29).

Register W0 through W30 are the 32-bit versions, and take up the *32 least significant bits* of the 64-bit registers.

### Zero Register

- The Zero register is XZR
- Reading it gives all zeros
- Any data written to it is thrown away

## The Stack Pointer Register

The stack pointer register **SP** is a special-purpose register 

It contains the *Stack Pointer*:

- which is the address of the top (low address) of the current function's stack frame
- Stack grows toward low addresses

<img width="500" height="386" alt="image" src="https://github.com/user-attachments/assets/56c42800-8424-46a9-b0e0-096cbe7d9318" />

## PSTATE

The processor state, or PSTATE, is not held in a single register and is not accessible directly.

It is a set of bits that represent...

- Condition flags set from instruction execution:
  - n (Negative), z (Zero), c (Carry), v (oVerflow)
  - Program often use the compare (cmp) instruction to set flags and then use the conditional branch instructions to execute the appropriate code
      - beg, bne, bla, bhi, ble, bge, ...

## Instruction Format

General format of AARCH64 instructions is:

    instr_name dest_reg, src_reg1, src_reg2    // 64-bit x registers for 64-bit operations
    instr_name dest_reg, src_reg1, immediate   // 32-bit w registers for 32-bit operations

**NOTE: You cannot "mix" the register sizes**



