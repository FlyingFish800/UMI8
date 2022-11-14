/*  Alexander Symons | Nov 6 2022 | Assembler | structs.h
 *  Contains the Instruction and Operand structs to facilitate parsing
 */

#ifndef STRUCTS
#define STRUCTS

// Mnemonic for instruction
typedef enum TokenType {NOP, LD} TokenType;

// Memory adressing mode for the instruction
typedef enum AccessingMode {NONE, IMMEDIATE, REGISTER, INDIRECT} AccessingMode;

// Operand for the instruciton. Contains adressing mode and value
typedef struct Operand {
    AccessingMode accesingMode;
    int value;
} Operand;

// Instruction token struct for parser
typedef struct Instruction {
    TokenType instructionType;
    Operand *operands;
} Instruction;

#endif