ORG #0x0
GLOBAL _start
;DB bytes, bytes
;DEFINE SP 0xFFFF - expands to 0xFFFF any time SP is used; macro

_start:
    LD A, #0xFF
_loop:
    LD B, #0x1
    ADD
    JC _end
    JP _loop
_end:
    JP #0xb