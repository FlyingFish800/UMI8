_lbl1:
    NOP
    NOP
    LD A, B
    LD B, #0x20
    LD [#42], B
    LD [#42], #0x20
_lbl2: