.ORG #0x0
.GLOBAL _start

_start:
    LD C, _serial_out
    LD _serial_out, C
    JP _start

.ORG #0xFEFF
_serial_out: