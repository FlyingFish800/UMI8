ORG #0x0
GLOBAL _start

_start:
    LD A, #0x42
    PUSH A
    LD C, #0xAA
    PUSH C
    CALL _targ_1
    POP A
    POP C
_end:
    JP _end

_targ_1:
    CALL #0x8080
    RET

ORG #0x8080
    LD C, #42
    RET