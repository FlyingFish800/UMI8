; Implement CALL I/_lbl as pseudo-operation
.MACRO CALL <_lbl>
    PPC         ; Push program counter
    JP _lbl     ; Jump to the subroutine
.END

; Implement PUSH reg as pseudo-operation
.MACRO PUSH <reg>
    PUT reg     ; Looses contents of reg
    PEEK reg    ; Reads back reg without popping
.END