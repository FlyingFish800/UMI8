; Comment   AI is used instead of RI for legacy
o0***, POL MIL, POH MIH, RD II CE, CR  ; NOP
o01***, PIL MIL, POH MIH, RD II CE, NO BI INC, BO MIH, BO MIL, RD AI, EOS AI INC, AO MIL, RD AI, CR  ; PEEK R
o02***, POL MIL, POH MIH, RD II CE, NO BI INC, BO MIH, BO MIL, RD AI, AO MIL, POL WR, EO AI, AO MIL, POH WR, EO AI, BO MIL, AO WR, CR  ; PPC I
o03***, POL MIL, POH MIH, RD II CE, NO BI INC, BO MIH, BO MIL, RD AI, EOS AI INC, AO MIL, RD PIH, EOS MIL INC, RD PIL, BO MIL, EOSP WR INC, POL PIL CE, POL PIL CE  ; RET
o04***, POL MIL, POH MIH, RD II CE, NO BI INC, BO MIH, BO MIL, RD BI, BO MIL, AO WR, NO AI INC, EO BI, AO MIL, BO WR, CR  ; PUT R
o05***, POL MIL, POH MIH, RD II CE, NO BI INC, BO MIH, BO MIL, RD AI, EOS AI INC, AO WR, AO MIL, RD AI, CR  ; POP R
o10***, POL MIL, POH MIH, RD II CE, RD AI CE, CR  ; LD R I
o11***, POL MIL, POH MIH, RD II CE, RD BI CE, RD MIH CE, BO MIL, RD AI, CR  ; LD R MA
o12***, POL MIL, POH MIH, RD II CE, RD BI CE, RD MIH CE, BO MIL, WR AO, CR  ; LD MA R
o20***, POL MIL, POH MIH, RD II CE, RD BI CE, RD PIH, BO PIL, CR  ; JP I
o21+**, POL MIL, POH MIH, RD II CE, RD BI CE, RD PIH, BO PIL, CR  ; JZ I (zero set)
o21-**, POL MIL, POH MIH, RD II CE, POL PIL CE, POL PIL CE, CR  ; JZ I (zero not set)
o22-**, POL MIL, POH MIH, RD II CE, RD BI CE, RD PIH, BO PIL, CR  ; JNZ I (zero set)
o22+**, POL MIL, POH MIH, RD II CE, POL PIL CE, POL PIL CE, CR  ; JNZ I (zero not set)
o23*+*, POL MIL, POH MIH, RD II CE, RD BI CE, RD PIH, BO PIL, CR  ; JN I (zero set)
o23*-*, POL MIL, POH MIH, RD II CE, POL PIL CE, POL PIL CE, CR  ; JN I (zero not set)
o24*-*, POL MIL, POH MIH, RD II CE, RD BI CE, RD PIH, BO PIL, CR  ; JNN I (zero set)
o24*+*, POL MIL, POH MIH, RD II CE, POL PIL CE, POL PIL CE, CR  ; JNN I (zero not set)
o25**+, POL MIL, POH MIH, RD II CE, RD BI CE, RD PIH, BO PIL, CR  ; JNC I (zero set)
o25**-, POL MIL, POH MIH, RD II CE, POL PIL CE, POL PIL CE, CR  ; JNC I (zero not set)
o26**-, POL MIL, POH MIH, RD II CE, RD BI CE, RD PIH, BO PIL, CR  ; JN I (zero set)
o26**+, POL MIL, POH MIH, RD II CE, POL PIL CE, POL PIL CE, CR  ; JN I (zero not set)
o30***, POL MIL, POH MIH, RD II CE, RD BI CE, EO AI, CR  ; ADD I
o31***, POL MIL, POH MIH, RD II CE, RD BI CE, EOS AI INC, CR  ; SUB I (EOS for not, INC for +1 for 2's comp.)
o32**+, POL MIL, POH MIH, RD II CE, RD BI CE, EO AI INC, CR  ; ADDC I (carry set)
o32**-, POL MIL, POH MIH, RD II CE, RD BI CE, EO AI, CR  ; ADDC I (carry not set)
o33***, POL MIL, POH MIH, RD II CE, RD BI CE, EOS BI INC, CR  ; CMP I
o34***, POL MIL, POH MIH, RD II CE, NO BI, EO AI INC, CR  ; INC R
o35***, POL MIL, POH MIH, RD II CE, NO AI, CR  ; ZRO R
o37***, POL MIL, POH MIH, RD II CE, RD BI CE, BO AI, CR  ; LOGIC I (Used to implement & and |)