### UMI CPU Architecture
This repository contains all my work on the UMI CPU architecture

## Goals
My goal for this project is to create a very simple processor architecture that can be easily implemented on a breadboard. It contains an emulator for testing, assembler to write code for it, and will contain a logic simulator version, schematics, and software when complete.

My goals for this project are:
1. Be as simple as possible
2. Don't 'waste' any chips (I am limitng myself to 1 microcode eeprom)
3. Implement on breadboard with TTL logic

## 16 Bit vs 8 Bit, and V0.1 vs V0.2
My first attempt at this project used a 16 bit design that was overcome with feature creep. I used this oppertunity to restart the project in C while keeping true to the original goals. I chose an 8 bit design with 1 microcode eeprom for my design. The 1 microcode eeprom element has caused me to spend a lot of time optimizing the microcode for space, and led to a lot of interesting design decisions. Eventually, I got to a point where I realized my design was fundementally flawed, and started my third and final attempt; UMI 8 bit v0.2.

## Notable deisgn oddities
# 1 EEPROM
The current design has multiplexed control lines, broken up into the source register, destination register, and control chunks. This means each microstep can only move data from one register to another, and have one ALU/control modifier. 

# 16 Microsteps
I have only recently got the stack operations cut down enough to fit in 16 microsteps. Before, the CALL and PUSH instructions were the only instructions to have more than 16, and had a maximum of 22. I have split these instructions into two sub instructions each, and will use a pseudo-instruction macro in the assembler to re-add their functionality

# B inverts Sum out for odd instructions
To get the stack operations to fit, I needed to make the extra 'sum out pc enable' control line, since count enable increases both the MAR and PC. Using CE would have messed up the MAR address for updating the stack pointer. To get this control line, I consolidated sum out and sum out invert, and structured the instructions so those which requied addition and subtraction alternated. The result is that the invert line is tied to the least significant bit of the instruction.

# A and C
From my previous attempts I learned the importance of a scratch registerfor copying data from one place to another, that wasn't relied on by the user. This left my design with only one register which I could garuntee would not be altered by any other instructions for the user to use. I ended up using a spare bit in the instruction to swap out A for another register, C, for the duration of the instruction. The user **cannot** access both A and C in the same instruction, which is why they are one letter apart.

## TODOS:
# Emulator
- [ ] CPU flags arent fully working
- [x] Try to rewrite stack ops to make them shorter using new BO and EOS ctrl lines
- [ ] Get hardware devices implemented (partial)
- [ ] Get threading implemented so a terminal can be used (partial)
- [ ] Implement test suite

# Assembler
- [x] Make list of instructions that need to be generated after label dependencies met (track address)
- [ ] Implement Macros (partial)
- [ ] Implement preprocessing expression evaluation
- [x] Finish binary code generation (waiting on partial emulator rewrite)
- [ ] Figure out multi file assembly (linker?)

# Simulator
- [ ] Make simulator