/*  Alexander Symons | Nov 6 2022 | Assembler | Main.c
 *  Main driver program for UMI8 Assenbler
 */

#include <stdio.h>
#include "structs.h"

// Main entry point
int main(int argc, char *argv[]) {
    // Path to file to parse
    char *path = "../Asm/test.asm";

    // File pointer for file
    FILE *fp = fopen(path, "r");

    // Error checking
    if (fp == NULL){
        perror("File not found\n");
        return -1;
    }

    char c = fgetc(fp);
    do {
        printf("%c",c);
        c = fgetc(fp);
    } while (c != EOF);

    // Close file
    fclose(fp);

    printf("\n");

    // Return sucessful
    return 0;
}
