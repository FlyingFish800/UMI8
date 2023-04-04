#ifndef UMI8
#define UMI8
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
#define EOS 0b00000100
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

#endif