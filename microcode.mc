; Comment THIS ONE
o0***, POL MIL, POH MIH, RD II CE, CR  ; NOP
o1***, POL MIL, POH MIH, RD II CE, POL PIL RSA, AO AI, AO MIL, AO MIH, RD BI, EO MIL, POH WR, EO BI, EO MIL, POL WR, AO MIL, EO WR, POL MIL, POH MIH, RD AI CE, POL MIL, POH MIH, RD PIL, AO AI, AO PIH, CR ; CALL I
o2***, POL MIL, POH MIH, RD II CE, POL PIL RSA, AO AI, AO MIL, AO MIH, RD BI RSA, RD MIL, RD PIL, EO MIL INC, EO BI INC, RD PIH, EO BI INC, AO AI, AO MIL, AO AI, EO WR, POL PIL CE, POL PIL CE, CR ; RET
o3***, POL MIL, POH MIH, RD II CE, POL PIL RSA, AO AI, AO MIH, AO AI, AO MIL, EO WR, AO AI, AO MIL, RD BI, EO BI, AO AI, AO MIL, EO AI, AO AI, RD BI, AO MIL, POL PIL RSA, EO WR, AO AI, AO MIL, RD BI, EO WR, CR ; PUSH B
o10***, POL MIL, POH MIH, RD II CE, POL MIL, POH MIH, RD AI CE, CR ; LDA I
o11***, POL MIL, POH MIH, RD II CE, POL PIL RSA, EO AI, CR ; LDA B
o12***, POL MIL, POH MIH, RD II CE, POL MIL, POH MIH, RD BI CE, CR ; LDB I
o13***, POL MIL, POH MIH, RD II CE, AO BI, CR ; LDB A
o14***, POL MIL, POH MIH, RD II CE, POL MIL, POH MIH, RD AI CE, POL MIL, POH MIH, RD MIL CE, AO MIH, RD AI, CR  ; LDA M(I)
o17***, POL MIL, POH MIH, RD II CE, POL MIL, POH MIH, RD AI CE, POL MIL, POH MIH, RD MIL CE, AO MIH, POL PIL RSA, EO WR, CR  ; LDM(I) B
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
o31**+, POL MIL, POH MIH, RD II CE, EO AI INC, CR ; ADDC A (CARRY SET)
o31**-, POL MIL, POH MIH, RD II CE, EO AI, CR ; ADDC A (CARRY NOT SET)
o32***, POL MIL, POH MIH, RD II CE, POL PIL RSA, EO AI INC, CR ; INC B 
o33***, POL MIL, POH MIH, RD II CE, POL PIL RSA, CR ; ZRO A
o34***, POL MIL, POH MIH, RD II CE, EO AI INC, CR ; SUB A (INCREMENT IF ! BIT NOT SET)