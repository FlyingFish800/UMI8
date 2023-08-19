#ifndef OPS
#define OPS

#define OP_NOP  0x00
#define OP_CALL 0x21
#define OP_RET  0x22
#define OP_PSHB 0x23
#define OP_LDAI 0x08
#define OP_LDAB 0x09
#define OP_LDBI 0x0A
#define OP_LDBA 0x0B
#define OP_LDAM 0x0C
#define OP_LDMA 0x0D
#define OP_LDBM 0x0E
#define OP_LDMB 0x0F
#define OP_JPI  0x10
#define OP_JZI  0x11
#define OP_JNZI 0x12
#define OP_JNI  0x13
#define OP_JNNI 0x14
#define OP_JCI  0x15
#define OP_JNCI 0x16
#define OP_ADD  0x18
#define OP_ADDC 0x19
#define OP_INCB 0x1A
#define OP_ZROA 0x1B
#define OP_SUB  0x1C

#endif