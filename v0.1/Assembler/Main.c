/*  Alexander Symons | Nov 6 2022 | Assembler | Main.c
 *  Main driver program for UMI8 Assenbler
 */

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "lexer.h"
#include "codegen.h"

// Main entry point
int main(int argc, char *argv[]) {
    printf("NOTE: Assembler only accepts assembly files with all caps instructions\n");

    // Path to file to parse
    char *path = "../Asm_Src/test_execute.asm";
    if (argc > 1) path = argv[1];

    // File pointer for file
    FILE *fp = fopen(path, "r");

    // Error checking
    if (fp == NULL){
        perror("File not found\n");
        return -1;
    }

    Program program;
    parseProgram(fp, &program);
    if (program.length <= 0) return 0;

    // Close file
    fclose(fp);

    printf("\n----PARSED-OUTPUT----\n");
    for (size_t i = 0; i < program.length; i++) {
        printf("%s ", keywords[program.Instructions[i].instructionType]);
        for (size_t j = 0; j < program.Instructions[i].operandsLength; j++){
            printf("%s ", program.Instructions[i].operands[j].value);
        }
        printf("\n");
    }
    
    
    printf("\n----BEGINING_CODEGEN----\n");
    generateCode(&program, NULL);

    // Free parsed instructions
    for (int i = 0; i < program.length; i++) free(program.Instructions[i].operands);

    free(program.Instructions);
    
    
    // Return sucessful
    return 0;
}
