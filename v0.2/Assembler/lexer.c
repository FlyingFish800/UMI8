/*  Alexander Symons | Nov 6 2022 | Assembler | lexer.c
 *  Program containing all Assembly lexing functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "lexer.h"

// Returns true if character is whitespace
int isWhiteSpace(char c){
    return c == ' ' || c == '\t' || c == ',';
}

// Returns true if character is comment or end of line
int isComment(char c){
    return c == ';' || c == '\n';
}

// Skip whtie space to the next important section
void skipWhiteSpace(FILE *fp) {
    char c;
    
    // Keep getting chars until you get one that isnt whitespace
    do {
        c = fgetc(fp);
    } while (isWhiteSpace(c));

    // Then put the non-whitespace char back
    ungetc(c, fp);
}

// Skip everything until next newline or eof
void skipComments(FILE *fp) {
    char c;
    
    // Keep getting chars until you get one that isnt whitespace
    do {
        c = fgetc(fp);
        if (c == EOF) return;
    } while (c != '\n');

    // Then put the non-whitespace char back
    ungetc(c, fp);
}

// Reusable code for parsing, gets fp to next relevent block of code. Doesnt sknip \n or EOF. Returns true is EOF
int getNextBlock(FILE *fp){
    // Skip precceding whitespace
    skipWhiteSpace(fp);

    // Check if line is just comment or EOF
    char c = fgetc(fp);
    if (c == EOF) return 1;
    if (c == ';') {
        skipComments(fp);
        c = fgetc(fp); // \n
        if (c == EOF) return 1;
        ungetc(c,fp); // Put \n bacl
        return 0;
    } else {
        // Not comment, put char back
        ungetc(c, fp);
        return 0;
    }
}

// Push an operand to the Instruction's stack of operands. Return 1 to continue
int addOperand(Instruction *i, Operand o){
    i->operandsLength += 1;
    i->operands = realloc(i->operands, sizeof(Operand)*i->operandsLength);
    if (i->operands == NULL) {perror("COULDNT REALLOC OPERANDS"); return 0;}
    i->operands[i->operandsLength-1] = o;
    return 1;
}

// Push an instruction to the program (stack of instructions). Return 1 to continue
int addInstruction(Program *p, Instruction i, int returnCode){
    p->length += 1;
    p->Instructions = realloc(p->Instructions, sizeof(Instruction)*p->length);
    if (p->Instructions == NULL) {perror("COULDNT REALLOC INSTRUCTIONS"); return 0;}
    p->Instructions[p->length-1] = i;
    return returnCode;
}

// Read an instruction, and add it to the instructions pointer. Return 1 if there is more to parse, 0 for EOF
int parseInstruction(FILE *fp, Program *program){
    /* Handles lines that are just \n or comments.
     * 1: Skip white space and check if comment
     * 2: Read identifier
     * 3: Skip white space
     * 4: Read operands
     * 5: Repeat 3-4 as necessary (OR dont do them at all)
     * 6: Handle comments and newline(s), unget EOFs
     */

    // Get next block. If returns true for EOF, no more to parse
    if (getNextBlock(fp)) return 0;
    char c = fgetc(fp);
    if (c == '\n') return 1;

    // Read Identifier
    // Create variable, read char by char
    // TODO: Memsafe?
    int len = 0;
    char *id = malloc(0);
    do {
        len+=1;
        id = realloc(id,len*sizeof(char));
        if (id == NULL) {printf("ERROR, UNABLE TO REALLOC IN ID PARSING"); return 0;}
        id[len-1] = c;
        c = fgetc(fp);
    } while (!isWhiteSpace(c) && c != ';' && c != '\n' && c != EOF);
    ungetc(c, fp);

    // When buildign strings yourself, dont forget to null terminate them!
    id = realloc(id,(len+1)*sizeof(char));
    id[len] = '\0';

    // Prepare instruction for operands
    Instruction instruction;

    instruction.operandsLength = 0;
    instruction.operands = malloc(0);
    if (instruction.operands == NULL) {perror("FAILED INITIAL ALLOCATION OF OPERANDS"); return 0;}

    // Determine enum type
    int type;
    if(id[0] == '_') {
        id[len-1] = '\0';
        instruction.instructionType = keyword_to_type("LABEL");


        // Push label id as operand
        if(id[len-1] == ':') id[len-1] = '\0';
        Operand op;
        op.accesingMode = NONE;
        op.value = id;
        if(!addOperand(&instruction, op)) return 0;
    } else if ((type = keyword_to_type(id)) >= 0) {
        instruction.instructionType = type;
    } else {
        printf("UNIMPLEMENTED/INVALID INSTRUCTION <%s> IN PARSING\n",id);
        program->length = 0;
        program->length = -1;
        return 0;
    }

    printf("INS <%s>",id);

    // Parse next block
    Operand operand;
    while (1){ 
        // Get next block. If returns true for EOF, no more to parse. Pushes instruction to list    
        if (getNextBlock(fp)) return addInstruction(program, instruction, 0);
        c = fgetc(fp);
        if (c == '\n') return addInstruction(program, instruction, 1);

        int opLen = 0;
        char *opId = malloc(0);
        int string = (c == '"');

        do {
            opLen+=1;
            opId = realloc(opId,opLen*sizeof(char));
            if (opId == NULL) {printf("ERROR, UNABLE TO REALLOC IN ID PARSING"); return 0;}
            opId[opLen-1] = c;
            c = fgetc(fp);
            if (c == '"') string = 0;
        } while ((string || !isWhiteSpace(c)) && c != ';' && c != '\n' && c != EOF);
        ungetc(c, fp);

        // When buildign strings yourself, dont forget to null terminate them!
        opId = realloc(opId,(opLen+1)*sizeof(char));
        opId[opLen] = '\0';

        operand.value = opId;
        operand.accesingMode = 0;

        if(operand.value[0] == '_') {
            printf("ABS_LABEL");
            operand.accesingMode = ABSOLUTE_LABEL;
        } else if(operand.value[0] == 'A' || opId[0] == 'C') {
            printf("REG");
            operand.accesingMode = REGISTER;
        } else if(operand.value[0] == '#') {
            printf("IMM");
            operand.accesingMode = IMMEDIATE;
        } else if(operand.value[0] == '[') {
            if(operand.value[1] == '[') { // Double memory lookup [[#0xCAFE]]
                printf("REL");
                operand.accesingMode = RELATIVE;
            }  else if(operand.value[1] == '_') { // Memory lookup of label [_pointer]
                printf("REL_LABEL");
                operand.accesingMode = RELATIVE_LABEL;
            } else { // Single lookup of immediate [#0xCAFE]
                printf("ABS");
                operand.accesingMode = ABSOLUTE;
            }
        } else if(operand.value[0] == '"') {
            printf("STR");
            operand.accesingMode = STRING;
        } else if(operand.value[0] == '<') {
            printf("MACRO ARG");
            operand.accesingMode = MACRO_ARG;
        } else {
            printf("UNIMPLEMENTED/INVALID OPERAND <%s> IN PARSING\n",opId);
            program->length = -1;
            return 0;
        }

        printf(":%s ",operand.value);

        // Push operand to instruction. If it returns a 1, quit
        if(!addOperand(&instruction, operand)) return 0; 
    }
}

// Parse file given to function
void parseProgram(FILE *fp, Program *program){

    // Create program so parseInstructions can use size as an index into instruction array TODO: Implement program as stack
    program->length = 0;
    program->Instructions = malloc(0);
    if (program->Instructions == NULL) printf("FAILED INITIAL MALLOC FOR INSTRUCTIONS");

    // Get file pointer to first valid code block (or comment, which is handled in same code)
    skipWhiteSpace(fp);

    // ParseInstruction for every line of file, handles comments and "\n". breaks look when EOF encoutnered
    while (parseInstruction(fp, program)){
        printf("\n");
        /* code */
    }

    printf("\n");
}