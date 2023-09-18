#ifndef OPS
#define OPS

// Offset can be added to any instruction to use C instead of A
#define OP_ATOC  0x20 

#define OP_NOP    0x00 // NOP
#define OP_PEEKR  0x01 // Copy top value of stack to R, don't pop
#define OP_PPC    0x02 // Return from subroutine. Loses value of R
#define OP_RET    0x03 // Return from subroutine. Loses value of R
#define OP_PUTR   0x04 // Put value of R on to stack, destructive
#define OP_POPR   0x05 // Pop top value from stack to R
#define OP_LDAI   0x08 // LD A Immediate
#define OP_LDAMA  0x09 // LD A Memory Absolute
#define OP_LDMAA  0x0A // LD Memory Absolute A
#define OP_JPI    0x10 // Jump to immediate address
#define OP_JPZI   0x11 // Jump to immediate address if zero flag set
#define OP_JNZI   0x12 // Jump to immediate address if zero flag not set
#define OP_JPNI   0x13 // Jump to immediate address if negative flag set
#define OP_JNNI   0x14 // Jump to immediate address if negative flag not set
#define OP_JPCI   0x15 // Jump to immediate address if carry flag set
#define OP_JNCI   0x16 // Jump to immediate address if carry flag not set
#define OP_ADDIA  0x18 // Add immediate value to A
#define OP_SUBIA  0x19 // Subtract immediate value from A
#define OP_ADDCIA 0x1A // Add w/ carry immediate value to 
#define OP_CMPIA  0x1B // Compare immediate to A
#define OP_INCA   0x1C // Increment A
#define OP_ZROA   0x1D // Zero A register
#define OP_LOGIA  0x1F // Do logic operation on Immediate and A and store to A

#endif