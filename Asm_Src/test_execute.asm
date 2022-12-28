ORG #0x0
GLOBAL _start

_start:
    LD A, #0x42
_loop:
    LD B, #0x1
    SUB
    JZ _end
    JP _loop
_end:
    JP _end