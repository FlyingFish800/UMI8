/*  Alexander Symons | Nov 6 2022 | Assembler | lexer.h
 *  Contains the function definitions for Lexing functions
 */

#ifndef LEXER
#define LEXER

#include <stdio.h>
#include "structs.h"

Instruction *parseProgram(FILE *fp);
int parseInstruction(FILE *fp, Instruction* Instructions);

#endif