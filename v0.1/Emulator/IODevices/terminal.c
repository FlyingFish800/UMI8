#include "terminal.h"
#include <stdio.h>

byte recieveChar(){
    return (byte)getchar();
}

void sendChar(byte c){
    printf("%c",c);
}