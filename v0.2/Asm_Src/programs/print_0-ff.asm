.ORG #0x0
.GLOBAL _start

.INCLUDE "lib/stdlib.asm"

; NOTE: Calling macro within macro is weird

_start:
    ; Print 3 digits
    LD C, _d2
    CALL _print_digit
    LD C, _d1
    CALL _print_digit
    LD C, _d0
    CALL _print_digit

    ; \n\r
    LD C, #10
    LD _serial_out, C
    LD C, #13
    LD _serial_out, C

    ; Update counter, leave if 255
    LD C, _counter
    ADD C, #1
    CMP C, #0   ; Overflow after 255 is 0
    JZ _end
    LD _counter, C

    ; Update digit 0
    LD C, _d0
    ADD C, #1
    CMP C, #10
    JZ _overflow_0

    ; No overflow d0
    LD _d0, C
    JP _start

    ; digit 0 carry
_overflow_0:
    ; Zero digit 0
    ZERO C
    LD _d0, C

    ; Increment digit 1
    LD C, _d1
    ADD C, #1
    CMP C, #10
    JZ _overflow_1 

    ; No overflow d1
    LD _d1, C
    JP _start

    ; Digit 1 overflowed    
_overflow_1:
    ZERO C
    LD _d1, C
    LD C, _d2
    ADD C, #1
    LD _d2, C
    JP _start


_end:
    JP _end


_counter:
    .DB #0
_d0:
    .DB #0
_d1:
    .DB #0
_d2:
    .DB #0

; Print digit in C as ascii
_print_digit:
    ADD C, #48
    LD _serial_out, C
    SUB C, #48
    RET


.ORG #0xFEFF
_serial_out:
    