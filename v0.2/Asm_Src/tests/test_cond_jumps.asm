.ORG #0
.INCLUDE "lib/stdlib.asm"

.GLOBAL _start

_start:
    LD C, #48 ;0
    LD A, #42
    CMP A, #42
    JZ _JZOK  ; Works!
    JP _bad
_JZOK:
    LD C, #49 ;1
    CMP A, #43
    JNZ _JNZOK ; Works!
    JP _bad
_JNZOK:
    LD C, #50 ;2
    CMP A, #43
    JN _JNOK    ; 
    JP _bad
_JNOK:
    LD C, #51 ;3
    CMP A, #41
    JNN _JNNOK  ; 
    JP _bad
_JNNOK:
    LD C, #52 ;4
    ADD A, #214
    JC _JCOK 
    JP _bad
_JCOK:
    LD C, #53 ; 5
    LD A, #42
    ADD A, #213
    JNC _JNCOK 
    JP _bad
_JNCOK:
    LD C, #79

_end:
    LD _serial_out, C
    JP _end


_bad:
    LD _serial_out, C
    JP _bad

.ORG #0xFEFF
_serial_out: