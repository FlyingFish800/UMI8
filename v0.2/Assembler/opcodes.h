#ifndef OPS
#define OPS

// Offset can be added to any instruction to use C instead of A
#define OP_ATOC  0x20 

#define OP_NOP    0x00 // NOP
#define OP_CALLI  0x01 // CALL Immediate. Loses value of R
#define OP_RET    0x02 // Return from subroutine. Loses value of R
#define OP_LDAI   0x08 // LD A Immediate
#define OP_LDAMA  0x09 // LD A Memory Absolute
#define OP_LDMAA  0x0A // LD Memory Absolute A
#define OP_JPI    0x10 // Jump to immediate address
#define OP_ADDIA  0x18 // Add immediate value to A
#define OP_ADDCIA 0x19 // Add w/ carry immediate value to 
#define OP_INCA   0x1A // Increment A
#define OP_ZROA   0x1B // Zero A register
#define OP_SUBIA  0x1C // Subtract immediate value from A
#define OP_CMPIA  0x1D // Compare immediate to A
#define OP_LOGIA  0x1F // Do logic operation on Immediate and A and store to A

#endif