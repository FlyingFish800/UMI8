/*  Alexander Symons | Nov 6 2022 | Assembler | Main.c
 *  Main driver program for UMI8 Assenbler
 */

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "lexer.h"
#include "preprocessor.h"
#include "codegen.h"

// Main entry point
int main(int argc, char *argv[]) {
    printf("NOTE: Assembler only accepts assembly files with all caps instructions\n");

    // Get path of file to be parsed TODO: get from argv
    char *path = "../Asm_Src/programs/print_0-ff.asm";
    if (argc > 1) path = argv[1];

    // Open the file
    FILE *inFile;
    if ((inFile = fopen(path, "r")) == NULL){
        perror("Input file not found\n");
        return -1;
    }

    // Lex and preprocess program to intstructions
    printf("\n----BEGINNING-LEXING----\n");
    Program program;
    parseProgram(inFile, &program, NULL);     // Closes file
    if (program.length <= 0) return 0;

    // Print parsed output
    printf("\n----PARSED-OUTPUT----\n");
    for (size_t i = 0; i < program.length; i++) {
        printf("%s ", keywords[program.Instructions[i].instructionType]);
        for (size_t j = 0; j < program.Instructions[i].operandsLength; j++){
            printf("%s ", program.Instructions[i].operands[j].value);
        }
        printf("\n");
    }
    
    // Open output file TODO: get from argv
    FILE *outFile;
    if ((outFile = fopen("./a.bin", "w")) == NULL){
        perror("Output file not found\n");
        return -1;
    }

    // Generate machinecode
    printf("\n----BEGINING_CODEGEN----\n");
    generateCode(&program, outFile);
    fclose(outFile);

    // Free parsed instructions
    for (int i = 0; i < program.length; i++) free(program.Instructions[i].operands);
    free(program.Instructions);
    
    
    // Return sucessful
    return 0;
}
