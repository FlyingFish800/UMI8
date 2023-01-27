#include <strings.h>
#include "structs.h"

// Take in the name of a token and convert it to its respective type id using its index
int keyword_to_type(char *token){
    for (int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
        if (strcmp(keywords[i], token) == 0) return i;
    }
    return -1;
}