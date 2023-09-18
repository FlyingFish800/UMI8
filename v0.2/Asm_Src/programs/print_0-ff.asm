.ORG #0x0
.GLOBAL _start

.INCLUDE "lib/stdlib.asm"

; NOTE: Calling macro within macro is weird

_start:
    CALL _print_num
    ZERO C

    ; Inc _d2
    LD A, _d2
    ADD A, #1
    LD _d2, A

    ; Check overflow
    CMP A, #10
    JNZ _cont_d2 
    ; Digit 0 carry
    LD A, _d1
    ADD A, #1
    LD _d1, A
    LD _d0, C ; Zero overflowed digit
_cont_d2:

    ; Digit 1 carry
    LD A, _d2
    JNZ _cont_d1
    ; Digit 0 carry
    LD A, _d2
    ADD A, #1
    LD _d2, A
    LD _d2, C ; Zero overflowed digit
_cont_d1:

    ; Check number of iterations and repeat if not 255
    LD C, _dC
    CMP C, #255
    JZ _end

    ADD C, #1
    LD _dC, C
    JP _start


_end:
    JP _end


_dC:
    .DB #0

; Digits of number, 012 
_d0:
    .DB #0
_d1:
    .DB #0
_d2:
    .DB #0

; Print number from digits in order
_print_num:
    ; Print lowest digit
    LD C, _d0
    CALL _print_digit

    ;; Don't skip second digit if third digit is non-zero
    LD C, _d2
    CMP C, #0
    JNZ _no_skip_d1

    ;; Skip second digit if zero
    LD C, _d1
    CMP C, #0
    JZ _skp_d1
_no_skip_d1:
    
    ; Print second digit
    LD C, _d1 ; If skipped second digit check this is necessary
    CALL _print_digit
_skp_d1:

    ; Skip third digit if zero
    LD C, _d2
    CMP C, #0
    JZ _skp_d2
    CALL _print_digit ; Print
_skp_d2:

    LD C, #10
    LD _serial_out, C
    LD C, #13
    LD _serial_out, C
    RET


; Print digit in C as ascii
_print_digit:
    ADD C, #48
    LD _serial_out, C
    SUB C, #48
    RET


.ORG #0xFEFF
_serial_out:
    