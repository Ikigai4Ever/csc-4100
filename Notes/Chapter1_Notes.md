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
  - Programs often use the compare (cmp) instruction to set flags and then use the conditional branch instructions to execute the appropriate code
      - beg, bne, bla, bhi, ble, bge, ...

## Instruction Format

The general format of AARCH64 instructions is:

    instr_name dest_reg, src_reg1, src_reg2    // 64-bit x registers for 64-bit operations
    instr_name dest_reg, src_reg1, immediate   // 32-bit w registers for 32-bit operations

**NOTE: You cannot "mix" the register sizes**

    //This is the section where data can go
    //DIRECTIVES
    .section data 
    

    //This is where the actual code goes
    //INSTRUCTIONS
    .section  .text

### Sections

Declaring sections in **GAS assembly**
- `.data` is a read-write data section of initialized data
- `.rodata` is a read-only section of initialized data
- `.bss` is a read-write section of data initialized to zero (zeroed when loaded)
- `.text` is execute-only, program code

### Memory Labels 

Names are easy to remember, so we use **labels** that are names for locations in memory for us to use.

> Example:
>
>     .section .data 
>     my_int:     .word 1234  // .word and .int are the same size for AARCH64
>     my_int2:    .int 1234 
> 

### Global Symbols 

Declare main to be a **globally-visible label** 

Globally visible labels can be used outside of the file in which they were defined 

So, global variables and functions can be *exported*

    .section   .data
    length:      .int 1
    width:       .int 2
    perim:       .int 0

    .section .text
    .global main
    main:
        //main code here

## Branch Modifiers

<img width="1032" height="627" alt="image" src="https://github.com/user-attachments/assets/49b00183-e2f7-440a-b7f4-75e28dedf47a" />

## The Stack

SP (stack pointer) register points to the top of the stack, where the address is a multiple of 16

It can be used in `ldr`, `str`, and arithmetic instructions

The stack always needs to be grown by 16 bytes and nothing more/less

### Saving Stack Pointer

When going to a function, make sure to **push** X30 onto the stack when entering a function and **pop** X30 from the stack before returning to a function

    main:
        // Save X30 
        sub sp, sp, 16
        str x30, [sp]
        bl f
        // Restore X30
        ldr x30, [sp]
        add sp, sp, 16
        ret

    f:
        // Save X30
        sub sp, sp, 16
        str x30, [sp]
        bl g
        // Restore X30
        ldr x30, [sp]
        add sp, sp, 16
        ret

## Passing Arguments

Pass the first 8 (integer or address) arguments in registers:
- X0...X7 and/or W0...W7

If you have more than 8 arguments, pass them on the stack into the function

If arguments are structures (pass by value), then pass them on the stack


