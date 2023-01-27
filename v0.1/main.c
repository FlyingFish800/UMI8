#include "stdio.h"
#include "ansiicommands.h"   
#include <stdlib.h>

struct location {
    int x;
    int y;
    char content;
};

int main(){
    struct location l;
    l.content = 'x';
    l.x = 6;
    l.y = 10;

    ANSIRAW();

    ANSIMOVE(2,2);
    printf("Hello, World!\n");
    
    ANSIMOVE(4,1);

    putchar('X');
    for (size_t i = 0; i < 10; i++)
    {
        char c = getchar();
    }
    
    // %x hex, %d decimal
    //printf("\r\nChar %x\n",c);
    ANSICOOKED();
    return 0;
}