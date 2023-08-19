ORG #0x0
GLOBAL _start

; Move an 'A' to the output device, and read key into A reg
_start:
    LD A, #0x41
    LD _char, A
    LD A, _char
_end:
    JP _end

ORG #0xFEFF
_char: