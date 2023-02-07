#ifndef OPS
#define OPS

// Offset can be added to any instruction to use C instead of A
#define OP_ATOC  0x20 

#define OP_NOP   0x00 // NOP
#define OP_LDAI  0x08 // LD A Immediate
#define OP_LDAMA 0x09 // LD A Memory Absolute
#define OP_LDMAA 0x0A // LD Memory Absolute A
#define OP_JPI   0x10 // Jump to immediate address
#define OP_ADDI  0x18 // Add immediate value to A

#endif