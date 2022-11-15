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
    return c == ' ' || c == '\t';
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

// Read an instruction, and add it to the instructions pointer. Return 1 if there is more to parse, 0 for EOF
int parseInstruction(FILE *fp, Instruction *instructions){
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
        strncat(id, &c, 1);
        c = fgetc(fp);
    } while (!isWhiteSpace(c) && c != ';' && c != '\n' && c != EOF);
    ungetc(c, fp);


    Instruction instruction;
    if(id[0] == '_') {
        instruction.instructionType = LABEL;
    } else if (strcmp(id, "GLOBAL") == 0) {
        instruction.instructionType = GLOBAL;
    } else if (strcmp(id, "LD") == 0){
        instruction.instructionType = LD;
    } else if (strcmp(id, "RET") == 0){
        instruction.instructionType = RET;
    } else if (strcmp(id, "NOP") == 0){
        instruction.instructionType = NOP;
    } else {
        printf("UNIMPLEMENTED/INVALID INSTRUCTION <%s> IN PARSING\n",id);
        return 0;
    }

    printf("INS <%s>",id);


    // Parse next block
    while (1){ 
        // Get next block. If returns true for EOF, no more to parse. Pushes instruction to list    
        if (getNextBlock(fp)) return 0;
        c = fgetc(fp);
        if (c == '\n') return 1;

        // Read Identifier
        do {
            printf("%c",c);
            c = fgetc(fp);
        } while (!isWhiteSpace(c) && c != ';' && c != '\n' && c != EOF);
        ungetc(c, fp);
    }
}

// Parse file given to function
Program *parseProgram(FILE *fp){

    Instruction *instructions = malloc(0);
    if (instructions == NULL) printf("NO MEM");

    // Get file pointer to first valid code block (or comment, which is handled in same code)
    skipWhiteSpace(fp);
    

    // ParseInstruction for every line of file, handles comments and "\n". breaks look when EOF encoutnered
    while (parseInstruction(fp, instructions)){
        printf("\n");
        /* code */
    }
    
    Program *program;
    program->Instructions = instructions;

    printf("\n");

    printf("Instructions: %lu\n",sizeof(program->Instructions));
    // Parse individual instructions
    return program;
}