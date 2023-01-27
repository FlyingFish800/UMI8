#ifndef ANSI
#define ANSI
//Beware werid header guard naming stuff


// Macros, function signatures, and struct definitions go here
// Background colors
#define BBLACK "\x1B[40m"
#define BRED "\x1B[41m"
#define BGREEN "\x1B[42m"
#define BYELLOW "\x1B[43m"
#define BBLUE "\x1B[44m"
#define BMAGENTA "\x1B[45m"
#define BCYAN "\x1B[46m"
#define BWHITE "\x1B[47m"

// Foreground colors
#define RESET "\x1B[0m"
#define BLACK "\x1B[30m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"

#define ANSIMOVE(X, Y) printf("\x1B[%d;%dH", Y, X)
#define ANSICLEAR() printf("\x1B[2J")
#define ANSIRESET() printf("\x1B[0m")
#define ANSICOLOR(C, BC) printf("%s%s",C ,BC)
#define ANSIRAW() system("stty -echo raw")
#define ANSICOOKED() system("stty -echo cooked")

#endif