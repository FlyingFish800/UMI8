.ORG #0x0
.GLOBAL _start

.MACRO CALL <_lbl>
    PPC
    JP _lbl
.END

; How to do registers? immediates?
.MACRO PUSH <reg>
    ; Random names doen't work for marco args. _lbl only worked because it was a valid label bc of the _.
    ; Maybe have special char for macro args? Like %?
    PUT reg
    PEEK reg
.END

_start:
    CALL _targ
    PUSH C
_end:
    JP _end

_targ:
    LD C, #0x42
    RET