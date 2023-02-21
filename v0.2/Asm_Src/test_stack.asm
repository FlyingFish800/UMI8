ORG #0x0
GLOBAL _start

_start:
    CALL _targ_1
_end:
    JP _end

_targ_1:
    CALL #0x8080
    RET

ORG #0x8080
    LD C, #42
    RET