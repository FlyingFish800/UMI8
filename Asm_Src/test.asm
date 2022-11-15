; comment
GLOBAL _start ; This is a comment

; Another comment

_start:
    LD B, #0x42 
    LD [42], B ;
    RET