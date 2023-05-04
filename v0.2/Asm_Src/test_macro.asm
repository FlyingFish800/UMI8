.ORG #0x0
.GLOBAL _start

.MACRO PPC <_lbl>
    CALL _lbl
.END

_start:
    PPC _target
    ;ABC ; Shouldnt work
_loop:
    JP _loop

_target:
    LD C, #0x42
    RET