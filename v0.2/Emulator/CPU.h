#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <string.h>
#include <memory.h>

//#define DEBUG

// Uncomment for debug printfs
//#define DEBUG

// Masks
#define DST 0b11100000
#define SRC 0b00011100
#define ALU 0b00000011
#define RSEL  0b00100000
#define LDOR  0b01000000
#define LDAND 0b10000000

// Offsets
#define DST_SHFT 5
#define SRC_SHFT 2

// Ctrl lines

// DST strl lines
#define MIH 0b00000000
#define MIL 0b00100000
#define II  0b01000000
#define PIH 0b01100000
#define PIL 0b10000000
#define AI  0b10100000
#define BI  0b11000000
#define WR  0b11100000

// SRC ctrl lines
#define NO  0b00000000
#define EOP 0b00000100
#define POL 0b00001000
#define POH 0b00001100
#define AO  0b00010000
#define EO  0b00010100
#define RD  0b00011000
#define BO  0b00011100

// ALU ctrl lines
#define ADD 0b00000000
#define CE  0b00000001
#define INC 0b00000010
#define CR  0b00000011

// Flags
#define ZERO     0b00000100
#define NEGATIVE 0b00000010
#define CARRY    0b00000001

// Sizes of ram and UCode (64k and 8k respectively) (1 Higher because it doesnt include 64k, 0 thru 64k-1)
#define RAM_SIZE 0x10000
#define MCODE_SIZE 0x20000

// Location of 'UART' out
#define SERIAL_OUT  0xFEFF

// Core equality check flags
#define CHECK_PC    0b00000001
#define CHECK_B     0b00000010
#define CHECK_MAR   0b00000100
#define CHECK_IR    0b00010000
#define CHECK_FLAGS 0b00100000
#define CHECK_A     0b01000000
#define CHECK_C     0b10000000
#define CHECK_All   0b11111111
#define CHECK_EQL   CHECK_A && CHECK_C && CHECK_B && CHECK_FLAGS

// LUT for instruction names
const static char *INSTRUCTION_NAMES[] = {"NOP",       "PEEK R",   "PPC",      "RET",    "PUT R",    "POP R",     "",     "", 
                            "LD R I",    "LD R MA",  "LD MA R",  "LD R MR",  "LD MR R",  "",     "",     "",
                            "JP I",      "JPZ I",    "JNZ I",    "JPC I",    "PNC I",    "JPN I","JNN I","",
                            "ADD I",     "SUB I",   "ADDC I",    "CMP I",    "INC R",    "ZRO R","",     "LOGIC I"};

// 8 bit number
typedef unsigned char byte;

// 16 bit number
typedef __uint16_t word;

// Structure for IODevice that contains its adress, and the pointers to the read and write functions for the cpu
typedef struct IODevice {
    word address;
    byte (*readDevice)();
    void (*writeDevice)(byte);
} IODevice;

// CPU internal registers
typedef struct CPU {
    // Memory Adress Registers (HI byte and LOW byte)
    byte MARHI;
    byte MARLO;

    // Program Counter Registers (HI byte and LOW byte)
    byte PCHI;
    byte PCLO;

    // Instruction register
    byte IR;

    // General purpose registers
    byte *REG_ACTIVE;
    byte A;
    byte C;

    // Special scratch register
    byte B;

    // Internal Bus
    byte BUS;

    // UCode step
    byte UStep;

    // Flag register, XXXXXZNC
    byte Flags;

    // 64K of ram
    byte RAM[RAM_SIZE];

    // 8K of microcode (13 bits)
    byte Microcode[MCODE_SIZE];

    // Track how many hardware devices there are. Always points to next index
    word deviceCount;

    // Array of assigned IODevices
    IODevice devices[RAM_SIZE];
} CPU;

// Function to emulate one clock cycle
void clock_core(CPU* cpu);

// Function to reset internal state of the CPU
void reset(CPU* cpu);

// Dumps CPU core state to stdio
void coreDump(CPU* cpu);

// Load program into ram from a file
void loadRam(CPU* cpu, char* path);

// Load microcode from a file
void loadUCode(CPU* cpu, char* path);

// Register an IO device to the processor TODO: deregister?
void registerDevice(CPU* cpu, IODevice* device);

// Print decoded ctrl lines. Used internally.
void dbgCtrlLine(byte);

// Dumps CPU core state to stdio
void copy_core(CPU* src, CPU* dst);

// Dumps CPU core state to stdio
char check_cores(CPU* core1, CPU* core2, char flags);

#endif