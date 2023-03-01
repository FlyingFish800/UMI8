ORG #0x0
GLOBAL _start

_start:
    LD A, _serial_out
    CMP A, #0x41
    JZ _char_out
    JP _start
_char_out:
    LD A, _serial_out
_end:
    JP _end

ORG #0xFEFF
_serial_out: