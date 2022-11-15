/*  Alexander Symons | Nov 6 2022 | Assembler | lexer.h
 *  Contains the function definitions for Lexing functions
 */

#ifndef LEXER
#define LEXER

#include <stdio.h>
#include "structs.h"

Program *parseProgram(FILE *fp);

#endif