; Comment THIS ONE
o0***, POL MIL, POH MIH, RD II CE, CR  ; NOP
o1***, POL MIL, POH MIH, RD II CE, POL PIL RSA, AO AI, AO MIL, AO MIH, RD BI, EO MIL, POH WR, EO BI, EO MIL, POL WR, AO MIL, EO WR, POL MIL, POH MIH, RD AI CE, POL MIL, POH MIH, RD PIL, AO AI, AO PIH, CR ; CALL I
o2***, POL MIL, POH MIH, RD II CE, POL PIL RSA, AO AI, AO MIL, AO MIH, RD BI RSA, RD MIL, RD PIL, EO MIL INC, EO BI INC, RD PIH, EO BI INC, AO AI, AO MIL, AO AI, EO WR, POL PIL CE, POL PIL CE, CR ; RET
o10***, POL MIL, POH MIH, RD II CE, POL MIL, POH MIH, RD AI CE, CR ; LDA I
o12***, POL MIL, POH MIH, RD II CE, POL MIL, POH MIH, RD BI CE, CR ; LDB I
o20***, POL MIL, POH MIH, RD II CE, POL MIL, POH MIH, RD AI CE, POL MIL, POH MIH, RD PIL, AO PIH, CR ; JP I
o30***, POL MIL, POH MIH, RD II CE, EO AI, CR ; ADD A
o31***, POL MIL, POH MIH, RD II CE, EO AI INC, CR ; ADDC A
o32***, POL MIL, POH MIH, RD II CE, POL PIL RSA, EO AI INC, CR ; INC B 
o33***, POL MIL, POH MIH, RD II CE, POL PIL RSA, CR ; ZRO A