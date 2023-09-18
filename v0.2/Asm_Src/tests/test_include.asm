.INCLUDE "lib/stdlib.asm"

.ORG #0x0
.GLOBAL _start

_start:
    CALL _targ
_end:
    JP _end

_targ:
    LD C, #0x42
    RET
