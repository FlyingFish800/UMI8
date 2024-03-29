#ifndef PREPROCESSOR
#define PREPROCESSOR

#include "structs.h"

// Errors parser could encounter,
enum parseErrors {
    PARSE_ERROR_NONE = 0,
    PARSE_CONTINUE, // 1 so conditionals interpret True
    PARSE_ERROR_EOF,
    PARSE_ERROR_REALLOC,
    PARSE_ERROR_MALLOC,
    PARSE_ERROR_ADD_OEPRAND,
    PARSE_ERROR_INVALID_MACRO,
    PARSE_ERROR_INVALID_OPERAND,
    PARSE_ERROR_INVALID_INSTRUCTION
};

int registerMacroOperand(Macro *m, Operand o);

int registerMacro(MacroTable *mt, Macro m, int returnCode);

int preprocessProgram(Program *program, MacroTable *macros, Program *processed);

#endif