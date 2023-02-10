; Used to test lexer stage
ORG #0x0
GLOBAL _start

_start:
    LD A, #0xFF ; TODO: sometimes doent work? Got 0xfd once
    LD C, #0x2
    ;LD _var, A
    ;LD [#0xCAFE], C
    ;LD [_var], A
    ;LD [[#0xFFFF]], A
    JP _start

_var:
    DB #0x42