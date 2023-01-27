ORG #0x0
GLOBAL _start

_start:
    LD A, #0x41
    LD _char, A
    LD B, _char
    LD A, B
_end:
    LD B, A
    JP _end

ORG #0xFEFF
_char: