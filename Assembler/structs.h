/*  Alexander Symons | Nov 6 2022 | Assembler | structs.h
 *  Contains the Instruction and Operand structs to facilitate parsing
 */

#ifndef STRUCTS
#define STRUCTS

// Mnemonic for instruction
typedef enum TokenType {GLOBAL, ORG, LABEL, NOP, LD, RET, NUM_TYPES} TokenType;

// Memory adressing mode for the instruction
typedef enum AccessingMode {NONE, IMMEDIATE, REGISTER, INDIRECT, INDIRECT_LABEL} AccessingMode;

// Operand for the instruciton. Contains adressing mode and value
typedef struct Operand {
    AccessingMode accesingMode;
    char *value;
} Operand;

// Instruction token struct for parser. Functions as a queue of Operands and a type
typedef struct Instruction {
    TokenType instructionType;
    int operandsLength;
    Operand *operands;
} Instruction;

// Program struct that contains all parsed instructions and fucntions as a queue
typedef struct Program {
    int length;
    Instruction *Instructions;
} Program;

// Label for tracking addresses
typedef struct Label {
    char *identifier;
    int address;
} Label;

// LabelTable struct that contains all labels as a queue
typedef struct LabelTable {
    int length;
    Label *labels;
} LabelTable;

#endif