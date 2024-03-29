/*  Alexander Symons | Nov 6 2022 | Assembler | structs.h
 *  Contains the Instruction and Operand structs to facilitate parsing
 */

#ifndef STRUCTS_H
#define STRUCTS_H

#include <string.h>

// Mnemonic for instructions/commands, and list of keywords to derive indexes
//typedef enum TokenType {GLOBAL, ORG, LABEL, NOP, CALL, RET, PUSH, LD, JP, JZ, JNZ, JPN, JNN, JPC, JNC, ADD, ADDC, INC, ZRO, SUB, NUM_TYPES} TokenType;
static char *keywords[] = {".GLOBAL", ".ORG", "LABEL", "INVOKE_MACRO", ".DB", ".INCLUDE", ".MACRO", ".END", ".ASCII", "NOP", "PEEK", "PPC", "RET", "PUT", "POP", "LD", "JP", "JZ", "JNZ", "JN", "JNN", "JC", "JNC", "DBG", "ADD", "SUB", "ADDC", "CMP", "INC", "ZERO"};
int keyword_to_type(char *token);

// Memory adressing mode for the instruction
typedef enum AccessingMode {NONE, IMMEDIATE, STRING, REGISTER, ABSOLUTE, ABSOLUTE_LABEL, RELATIVE, RELATIVE_LABEL, MACRO_ARG, MACRO_ID, EXPRESSION} AccessingMode;

// Operand for the instruciton. Contains adressing mode and value
typedef struct Operand {
    AccessingMode accesingMode;
    char *value;
} Operand;

// Instruction token struct for parser. Functions as a queue of Operands and a type
typedef struct Instruction {
    int instructionType;
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

// Macro to be expanded into body
typedef struct Macro {
    char *identifier;
    int operandsLength;
    Operand *operands;
    Program body;
} Macro;

// MacroTable struct that contains all macros as a queue
typedef struct MacroTable {
    int memoryLength;
    int length;
    Macro *macros;
} MacroTable;


int addOperand(Instruction *i, Operand o);

int addInstruction(Program *p, Instruction i, int returnCode);

#endif