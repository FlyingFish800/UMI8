; Comment
o0***, POL MIL, POH MIH, RD II CE, CR  ; NOP
o1***, POL MIL, POH MIH, RD II CE, POL PIL RSA, AO AI, AO MIH, RD BI, EO MIL, POH WR, EO BI, EO MIL, POL WR, AO MIL, EO WR, POL MIL, POH MIH, RD PIL, CR ; CALL I
o10***, POL MIL, POH MIH, RD II CE, POL MIL, POH MIH, RD AI CE, CR ; LDA I
o12***, POL MIL, POH MIH, RD II CE, POL MIL, POH MIH, RD BI CE, CR ; LDB I
o20***, POL MIL, POH MIH, RD II CE, POL MIL, POH MIH, RD AI CE, POL MIL, POH MIH, RD PIL, AO PIH, CR ; JP I
o21+**, POL MIL, POH MIH, RD II CE, POL MIL, POH MIH, RD AI CE, POL MIL, POH MIH, RD PIL, AO PIH, CR ; JPZ I (SET)
o21-**, POL MIL, POH MIH, RD II CE, POL PIL CE, POL PIL CE, CR  ; JPZ I (NOT SET)
o22+**, POL MIL, POH MIH, RD II CE, POL PIL CE, POL PIL CE, CR  ; JNZ I (SET)
o22-**, POL MIL, POH MIH, RD II CE, POL MIL, POH MIH, RD AI CE, POL MIL, POH MIH, RD PIL, AO PIH, CR ; JNZ I (NOT SET)
o23*+*, POL MIL, POH MIH, RD II CE, POL MIL, POH MIH, RD AI CE, POL MIL, POH MIH, RD PIL, AO PIH, CR ; JPN I (SET)
o23*-*, POL MIL, POH MIH, RD II CE, POL PIL CE, POL PIL CE, CR  ; JPN I (NOT SET)
o24*+*, POL MIL, POH MIH, RD II CE, POL PIL CE, POL PIL CE, CR  ; JNN I (SET)
o24*-*, POL MIL, POH MIH, RD II CE, POL MIL, POH MIH, RD AI CE, POL MIL, POH MIH, RD PIL, AO PIH, CR ; JNN I (NOT SET)
o25**+, POL MIL, POH MIH, RD II CE, POL MIL, POH MIH, RD AI CE, POL MIL, POH MIH, RD PIL, AO PIH, CR ; JPC I (SET)
o25**-, POL MIL, POH MIH, RD II CE, POL PIL CE, POL PIL CE, CR  ; JPC I (NOT SET)
o26**+, POL MIL, POH MIH, RD II CE, POL PIL CE, POL PIL CE, CR  ; JNC I (SET)
o26**-, POL MIL, POH MIH, RD II CE, POL MIL, POH MIH, RD AI CE, POL MIL, POH MIH, RD PIL, AO PIH, CR ; JNC I (NOT SET)
o30***, POL MIL, POH MIH, RD II CE, EO AI, CR ; ADD A
o31***, POL MIL, POH MIH, RD II CE, EO AI INC, CR ; ADDC A
o32***, POL MIL, POH MIH, RD II CE, POL PIL RSA, EO AI INC, CR ; INC B 
o33***, POL MIL, POH MIH, RD II CE, POL PIL RSA, CR ; ZRO A