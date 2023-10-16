.ORG #0x0
.GLOBAL _start

.INCLUDE "lib/stdlib.asm"

; NOTE: Calling macro within macro is weird

_start:

_end:
    JP _end


_print_char:



_i0:
    .DB _str
_str:   ; Hello World!\r\n
    .ASCII "Hello World!"
    .DB #0x0D
    .DB #0x0A
    .DB #0