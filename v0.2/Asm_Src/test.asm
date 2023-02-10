ORG #0x0

; comment
ORG #0x0000

GLOBAL _start ; This is a comment

; Another comment

_start:
    LD A, #0x42 
    LD [#42], A ;
    RET