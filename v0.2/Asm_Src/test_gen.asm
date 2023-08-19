; Used to test lexer stage
ORG #0x0
GLOBAL _start

_start:
    ;LD A, #0xFF ; TODO: sometimes doent work? Got 0xfd once
    ;ADD A, #0x1
    ;ADDC A, #0x1
    LD A, #0x41
    LD [#0xFEFF], A
    ;LD _var, A
    ;LD C, _var
    ;SUB C, #0x42
    ;LD C, #0x2
    ;LD A, _var
    ;LD A, [#0x100]
    ;LD _var, A
    ;LD [#0xCAFE], C
    ;LD [_var], A
    ;LD [[#0xFFFF]], A
    JP _start

_var:
    DB #0x42