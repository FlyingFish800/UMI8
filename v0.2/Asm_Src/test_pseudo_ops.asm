.ORG #0x0
.GLOBAL _start

.MACRO CALL <_lbl>
    PPC
    JP _lbl
.END

; How to do registers? immediates?
;.MACRO PUSH <reg>
;    PUT reg
;    PEEK reg
;.END

_start:
    CALL _targ
_end:
    JP _end

_targ:
    LD C, #0x42
    RET