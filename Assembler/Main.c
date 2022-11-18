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
    printf("UMI8 Assembler only accepts assembly files with all caps instructions");

    // Path to file to parse
    char *path = "../Asm_Src/test.asm";

    // File pointer for file
    FILE *fp = fopen(path, "r");

    // Error checking
    if (fp == NULL){
        perror("File not found\n");
        return -1;
    }

    Program program;
    parseProgram(fp, &program);

    // Close file
    fclose(fp);

    printf("----\n");
    for (size_t i = 0; i < program.length; i++) {
        printf("%i", program.Instructions[i].instructionType);
        for (size_t j = 0; j < program.Instructions[i].operandsLength; j++){
            printf("%s ", program.Instructions[i].operands[j].value);
        }
        printf("\n");
    }
    
    
    printf("Begining Codegen...\n");
    generateCode(&program, NULL);


    // Free parsed instructions
    for (int i = 0; i < program.length; i++) free(program.Instructions[i].operands);

    free(program.Instructions);
    
    
    // Return sucessful
    return 0;
}
